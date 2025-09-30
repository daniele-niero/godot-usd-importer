#include "mesh_utils.h"
#include "utils.h"

#include <pxr/usd/usdGeom/primvarsAPI.h>


PXR_NAMESPACE_USING_DIRECTIVE
using namespace godot;
namespace godot_usd_importer {

MeshData::MeshData(const UsdGeomMesh& usd_mesh) {
    if (!usd_mesh.GetPointsAttr().Get(&usd_points) ||
        !usd_mesh.GetFaceVertexCountsAttr().Get(&usd_face_vertex_counts) ||
        !usd_mesh.GetFaceVertexIndicesAttr().Get(&usd_face_vertex_indices))
    {
        print_error("USD Import: Failed to get essential mesh attributes");
        _is_valid = false;
        return;
    }

    if (usd_mesh.GetNormalsAttr().Get(&usd_normals)) {
        usd_normal_interp = usd_mesh.GetNormalsInterpolation();
    }
    usd_mesh.GetOrientationAttr().Get(&usd_face_orientation_token);

    // ---- UVs ------------------------------------------------------------------------------
    const UsdGeomPrimvarsAPI primvars(usd_mesh);
    
    int main_found = 0;
    std::vector<UsdGeomPrimvar> primvar_list = primvars.GetPrimvars();
    for (const auto& pv : primvar_list) {
        if (pv.GetTypeName() != SdfValueTypeNames->TexCoord2fArray)
            continue;

        // if primvars with these names exists, they have precedence, they are considered the "main"
        // "st" is actually pixar convention for Uvs
        // "UVMap" is how blenders calls the uv primvar
        // and finally "uv" is more generic, commonly used name across DCCs.
        const TfToken name = pv.GetBaseName();
        const bool is_main = (name == TfToken("st") || 
                              name == TfToken("uv") || 
                              name == TfToken("UVMap"));
        
        if (is_main) {
            // ensure main uvs have precedence over other texCoord2f primvars
            main_found++;
            if (usd_uv_primvars.empty()) {
                usd_uv_primvars.push_back(pv);
            } else {
                if (usd_uv_primvars.size() == 1) {
                    usd_uv_primvars.resize(2);
                }
                usd_uv_primvars[1] = usd_uv_primvars[0];
                usd_uv_primvars[0] = pv;
            }
            if (main_found == 2)
                break;
        } 
        else {
            if (usd_uv_primvars.size() < 2) {
                usd_uv_primvars.push_back(pv);
            }    
        }
    }

    //  --------------------------------------------------------------------------------------

    _is_valid = true;
}


MeshData::GodotVertexStrategy MeshData::get_vertex_strategy() const {
    if (!_is_valid) {
        return MeshData::GodotVertexStrategy::Unknown;
    }

    bool merged_vertices = (usd_normals.empty() || 
                            usd_normal_interp == UsdGeomTokens->vertex || 
                            usd_normal_interp == UsdGeomTokens->varying || 
                            usd_normal_interp == UsdGeomTokens->constant);
    if (merged_vertices) {
        for (const UsdGeomPrimvar &uv : usd_uv_primvars) {
            if (uv.GetInterpolation() != UsdGeomTokens->vertex || 
                uv.GetInterpolation() != UsdGeomTokens->varying || 
                uv.GetInterpolation() != UsdGeomTokens->constant)
                merged_vertices = false;
        }
    }

    if (merged_vertices)
        return MeshData::GodotVertexStrategy::Merge;
    else
        return MeshData::GodotVertexStrategy::Split;
}


//--------------------------------------------------------------------------------------//
//                                Godot Arrays building                                 //
//--------------------------------------------------------------------------------------//

static std::vector<int> make_range(int start, int end) {
    std::vector<int> v(end - start);
    std::iota(v.begin(), v.end(), start);
    return v;
}


std::vector<int> MeshData::get_face_primvar_indices(
        const std::vector<int> &current_face_vertex_indices, 
        const int current_face_vertex_count, 
        const int vertex_offset,
        const int usd_face_idx,
        const TfToken &interpolation) const 
{
    std::vector<int> indices;

    if (interpolation == UsdGeomTokens->faceVarying) {
        indices = make_range(vertex_offset, vertex_offset + current_face_vertex_count);

        if (reverse_winding()) {
            std::reverse(indices.begin(), indices.end());
        }
    }
    else if (interpolation == UsdGeomTokens->uniform) {
        indices = std::vector<int>(current_face_vertex_count, usd_face_idx);
    }
    else if (interpolation == UsdGeomTokens->constant) {
        indices = std::vector<int>(current_face_vertex_count, 0);
    }
    else if (interpolation == UsdGeomTokens->vertex || interpolation == UsdGeomTokens->varying) {
        indices = current_face_vertex_indices;
    }

    return indices;
}


bool MeshData::to_godot_arrays(GdMeshArrays &gd_arrays, const double &meters_per_unit, bool zup_to_yup) const {
    if (!_is_valid) return false;

    int vertex_offset = 0;

    // iterate through all the USD faces
    for (size_t usd_face_idx = 0; usd_face_idx < usd_face_vertex_counts.size(); ++usd_face_idx) {
        int current_face_vertex_count = usd_face_vertex_counts[usd_face_idx];

        std::vector<int> current_face_vertex_indices;
        current_face_vertex_indices.resize(current_face_vertex_count);
        for (int i = 0; i < current_face_vertex_count; ++i) {
            current_face_vertex_indices[i] = usd_face_vertex_indices[vertex_offset + i];
        }

        if (reverse_winding()) {
            std::reverse(current_face_vertex_indices.begin(), current_face_vertex_indices.end());
        }

        int number_of_triangles = current_face_vertex_count - 2;
        int usd_vertex_idx;
        int usd_normal_idx;

        std::vector<int> current_normals_indices = get_face_primvar_indices(
            current_face_vertex_indices, current_face_vertex_count, vertex_offset, usd_face_idx, usd_normal_interp);
        
        std::vector<int> uv1;
        std::vector<int> uv2;
        if (usd_uv_primvars.size() >= 1) {
            const UsdGeomPrimvar &uv = usd_uv_primvars[0];
            uv.IsIndexed();
            uv.GetIndices(VtIntArray *indices)
            uv1 = get_face_primvar_indices(
                current_face_vertex_indices, current_face_vertex_count, vertex_offset, usd_face_idx,
                usd_uv_primvars[0].GetInterpolation()
            );
        }
      

        // Godot can only have flat or smooth normals.
        // When smooth (for the whole mesh), we can have shared vertex between faces, and normals will be one per vertex.
        // We also need an indices array to define the point used by a face.
        // When flat normals, we need to duplicate vertices so each face has its own vertices and normals.
        
        int corner;
        for (int triangle_idx = 0; triangle_idx < number_of_triangles; ++triangle_idx) {
            if (get_vertex_strategy() == GodotVertexStrategy::Split) {
                // duplicated vertices between faces, each face's vertex has its own normal
                // * faceVarying: each vertex of each face has its own normal
                // * uniform: each vertex of each face has the same normal
                for (int i = 0; i < 3; ++i) {
                    if (i == 0) {
                        corner = 0;
                    } else {
                        corner = triangle_idx + i;
                    }

                    usd_vertex_idx = current_face_vertex_indices[corner];
                    if (zup_to_yup){
                        gd_arrays.vertices.append(to_godot(ztoy_rot * usd_points[usd_vertex_idx]) * meters_per_unit);
                    } else {
                        gd_arrays.vertices.append(to_godot(usd_points[usd_vertex_idx]) * meters_per_unit);
                    }
                    
                    if (!usd_normals.empty()) {
                        usd_normal_idx = current_normals_indices[corner];
                        if (zup_to_yup){
                            gd_arrays.normals.append(to_godot(ztoy_rot * usd_normals[usd_normal_idx]) * meters_per_unit);
                        } else {
                            gd_arrays.normals.append(to_godot(usd_normals[usd_normal_idx]) * meters_per_unit);
                        }
                    }

                    gd_arrays.indices.append(gd_arrays.vertices.size() - 1);
                }
            }
        
            else if (get_vertex_strategy() == GodotVertexStrategy::Merge) {
                // shared vertices between adjacent faces
                // * vertex: each vertex has its own normal
                // * constant: one single normal for all the vertices
                int start_idx = 0;
                if (triangle_idx > 0) {
                    start_idx = 2;
                }
                for (int i = start_idx; i < 3; ++i) {
                    if (triangle_idx > 0) {
                        // the shared vertices...
                        gd_arrays.indices.append(gd_arrays.vertices.size() - (2 + triangle_idx));
                        gd_arrays.indices.append(gd_arrays.vertices.size() - 1);
                    }

                    corner = triangle_idx + i;

                    usd_vertex_idx = current_face_vertex_indices[corner];
                    if (zup_to_yup){
                        gd_arrays.vertices.append(to_godot(ztoy_rot * usd_points[usd_vertex_idx]) * meters_per_unit);
                    } else {
                        gd_arrays.vertices.append(to_godot(usd_points[usd_vertex_idx]) * meters_per_unit);
                    }

                    if (!usd_normals.empty()) {
                        usd_normal_idx = current_normals_indices[corner];
                        if (zup_to_yup){
                            gd_arrays.normals.append(to_godot(ztoy_rot * usd_normals[usd_normal_idx]) * meters_per_unit);
                        } else {
                            gd_arrays.normals.append(to_godot(usd_normals[usd_normal_idx]) * meters_per_unit);
                        }
                    }
                    gd_arrays.indices.append(gd_arrays.vertices.size() - 1);
                }
            }   
        }

        vertex_offset += current_face_vertex_count;
    }


    return true;
}


}