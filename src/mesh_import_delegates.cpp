#include "mesh_import_delegates.h"
#include "basic_import_delegates.h"
#include "usd_visitor.h"
#include "utils.h"

#include "godot_cpp/classes/mesh_data_tool.hpp"
#include "godot_cpp/classes/surface_tool.hpp"
#include "godot_cpp/core/print_string.hpp"
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/cylinder_mesh.hpp>
#include <godot_cpp/classes/capsule_mesh.hpp>

#include "pxr/usd/usdGeom/metrics.h"
#include <pxr/usd/usdGeom/cube.h>
#include <pxr/usd/usdGeom/sphere.h>
#include <pxr/usd/usdGeom/cone.h>
#include <pxr/usd/usdGeom/cylinder.h>
#include <pxr/usd/usdGeom/capsule.h>
#include "pxr/usd/usdGeom/mesh.h"
#include <pxr/usd/usdGeom/primvarsAPI.h>


PXR_NAMESPACE_USING_DIRECTIVE
using namespace godot;
namespace godot_usd_importer {


Node3D* import_cube(const UsdPrim& usd_prim) {
    Ref<BoxMesh> box_mesh;
    box_mesh.instantiate();

    UsdGeomCube usd_cube(usd_prim);
    double size = 1.0;
    usd_cube.GetSizeAttr().Get(&size);

    box_mesh->set_size(Vector3(size, size, size));

    MeshInstance3D *mesh_instance = memnew(MeshInstance3D);
    mesh_instance->set_mesh(box_mesh);
    set_node_transform(mesh_instance, usd_prim);
    return mesh_instance;
}


Node3D* import_sphere(const UsdPrim& usd_prim) {
    Ref<SphereMesh> sphere_mesh;
    sphere_mesh.instantiate();

    double radius = 1.0;
    UsdGeomSphere usd_sphere(usd_prim);
    usd_sphere.GetRadiusAttr().Get(&radius);

    // Set the radius
    sphere_mesh->set_radius(radius);
    sphere_mesh->set_height(radius * 2); // Height is diameter in Godot

    MeshInstance3D *mesh_instance = memnew(MeshInstance3D);
    mesh_instance->set_mesh(sphere_mesh);
    set_node_transform(mesh_instance, usd_prim);
    return mesh_instance;
}


Node3D* import_cylinder(const UsdPrim& usd_prim) {
    Ref<CylinderMesh> cylinder_mesh;
    cylinder_mesh.instantiate();

    UsdGeomCylinder usd_cylinder(usd_prim);
    double radius = 1.0;
    double height = 2.0;
    usd_cylinder.GetRadiusAttr().Get(&radius);
    usd_cylinder.GetHeightAttr().Get(&height);

    cylinder_mesh->set_top_radius(radius);
    cylinder_mesh->set_bottom_radius(radius);
    cylinder_mesh->set_height(height);

    MeshInstance3D *mesh_instance = memnew(MeshInstance3D);
    mesh_instance->set_mesh(cylinder_mesh);
    set_node_transform(mesh_instance, usd_prim);
    return mesh_instance;
}


Node3D* import_capsule(const UsdPrim& usd_prim) {
    Ref<CapsuleMesh> capsule_mesh;
    capsule_mesh.instantiate();

    UsdGeomCapsule usd_capsule(usd_prim);
    double radius = 0.5;
    double height = 2.0;
    usd_capsule.GetRadiusAttr().Get(&radius);
    usd_capsule.GetHeightAttr().Get(&height);

    capsule_mesh->set_radius(radius);
    capsule_mesh->set_height(height * 2);

    MeshInstance3D *mesh_instance = memnew(MeshInstance3D);
    mesh_instance->set_mesh(capsule_mesh);
    set_node_transform(mesh_instance, usd_prim);
    return mesh_instance;
}


Node3D* import_cone(const UsdPrim& usd_prim) {
    Ref<CylinderMesh> cone_mesh;
    cone_mesh.instantiate();

    double radius = 1.0;
    double height = 2.0;
    UsdGeomCone usd_cone(usd_prim);
    usd_cone.GetRadiusAttr().Get(&radius);
    usd_cone.GetHeightAttr().Get(&height);

    cone_mesh->set_top_radius(0.0);
    cone_mesh->set_bottom_radius(radius);
    cone_mesh->set_height(height);

    MeshInstance3D *mesh_instance = memnew(MeshInstance3D);
    mesh_instance->set_mesh(cone_mesh);
    set_node_transform(mesh_instance, usd_prim);
    return mesh_instance;
}

void register_basic_solids() {
    UsdVisitorRegistry& registry = UsdVisitorRegistry::get_instance();

    registry.register_delegate(TfToken("UsdGeomCube"), import_cube);
	registry.register_delegate(TfToken("UsdGeomSphere"), import_sphere);
    registry.register_delegate(TfToken("UsdGeomCylinder"), import_cylinder);
    registry.register_delegate(TfToken("UsdGeomCone"), import_cone);
    registry.register_delegate(TfToken("UsdGeomCapsule"), import_capsule);
}


// static int get_indx_of_interpolated_attribute(TfToken &interpolation_token, int usd_face_idx, int usd_point_idx, int usd_vertex_idx)
// {
//     int indx =
//         interpolation_token == UsdGeomTokens->constant    ? 0              :
//         interpolation_token == UsdGeomTokens->uniform     ? usd_face_idx   :
//         interpolation_token == UsdGeomTokens->vertex      ? usd_point_idx  :
//         interpolation_token == UsdGeomTokens->faceVarying ? usd_vertex_idx :
//                                                             -1;
//     return indx;
// }

// Helper to extract all relevant Usd mesh data
static bool get_usd_mesh_data(
    const UsdGeomMesh& usd_mesh,
    VtArray<GfVec3f>& usd_points,
    VtArray<GfVec3f>& usd_normals,
    VtArray<GfVec2f>& usd_uv_map,
    VtArray<int>& usd_face_vertex_counts,
    VtArray<int>& usd_face_vertex_indices,
    TfToken& usd_face_orientation_token,
    UsdGeomPrimvar& usd_uv_primvar,
    TfToken& usd_normal_interp,
    TfToken& usd_uv_interp
) {
    if (!usd_mesh.GetPointsAttr().Get(&usd_points) ||
        !usd_mesh.GetFaceVertexCountsAttr().Get(&usd_face_vertex_counts) ||
        !usd_mesh.GetFaceVertexIndicesAttr().Get(&usd_face_vertex_indices))
    {
        print_error("USD Import: Failed to get essential mesh attributes");
        return false;
    }

    usd_mesh.GetNormalsAttr().Get(&usd_normals);
    usd_mesh.GetOrientationAttr().Get(&usd_face_orientation_token);

    const UsdGeomPrimvarsAPI primvars(usd_mesh);
    usd_uv_primvar = UsdGeomPrimvar();
    std::vector<UsdGeomPrimvar> primvar_list = primvars.GetPrimvars();
    for (const auto& pv : primvar_list) {
        TfToken primvar_name = pv.GetBaseName();
        if (!usd_uv_primvar && (primvar_name == TfToken("st") || primvar_name == TfToken("uv") || primvar_name == TfToken("UVMap") || pv.GetTypeName() == SdfValueTypeNames->TexCoord2fArray)) {
            usd_uv_primvar = pv;
        }
        if (usd_uv_primvar) break;
    }

    if (usd_mesh.GetNormalsAttr().Get(&usd_normals)) {
        usd_normal_interp = usd_mesh.GetNormalsInterpolation();
    }

    usd_uv_interp = usd_uv_primvar.IsDefined() ? usd_uv_primvar.GetInterpolation() : UsdGeomTokens->vertex;
    usd_uv_map = get_primvar_vec2f_array(usd_uv_primvar);

    return true;
}


Node3D* import_mesh(const UsdPrim &usd_prim) {
    TfToken up_axis = UsdGeomGetStageUpAxis(usd_prim.GetStage());
    unsigned int z_up = 0;
    if (up_axis == UsdGeomTokens->z)
        z_up = 1;

    UsdGeomMesh usd_mesh(usd_prim);
    // variables that will hold the data from the USD mesh
    VtArray<GfVec3f> usd_points;
    VtArray<GfVec3f> usd_normals;
    VtArray<GfVec2f> usd_uv_map;
    VtArray<int> usd_face_vertex_counts;
    VtArray<int> usd_face_vertex_indices;
    TfToken usd_face_orientation_token;
    UsdGeomPrimvar usd_uv_primvar;
    TfToken usd_normal_interp;
    TfToken usd_uv_interp;

    // get data from the USD mesh
    if (!get_usd_mesh_data(usd_mesh, usd_points, usd_normals, usd_uv_map, usd_face_vertex_counts, usd_face_vertex_indices, usd_face_orientation_token, usd_uv_primvar, usd_normal_interp, usd_uv_interp)) {
        print_error("USD Import: couldn't generate ArrayMesh from USD mesh data");
        return nullptr;
    }

    bool reverse_winding = (usd_face_orientation_token != TfToken("leftHanded"));

    // Arrays for the new Godot mesh
    PackedVector3Array gd_vertices;
    PackedVector3Array gd_normals;
    PackedVector2Array gd_uvs;
    PackedColorArray gd_colors;
    PackedInt32Array gd_indices;

    int vertex_offset = 0;

    for (size_t usd_face_index = 0; usd_face_index < usd_face_vertex_counts.size(); ++usd_face_index) {
        int current_face_vertex_count = usd_face_vertex_counts[usd_face_index];

        VtArray<int> current_face_vertex_indices;
        current_face_vertex_indices.resize(current_face_vertex_count);
        for (int i = 0; i < current_face_vertex_count; ++i) {
            current_face_vertex_indices[i] = usd_face_vertex_indices[vertex_offset + i];
        }

        if (reverse_winding) {
            std::reverse(current_face_vertex_indices.begin(), current_face_vertex_indices.end());
        }

        // Godot can only have flat or smooth normals.
        // When smooth (for the whole mesh), we can have shared vertex between faces and normals will be one per vertex.
        // We also use an indices array to define the point used by a face.
        // When flat normals, we need to duplicate vertices so each face has its own vertices and normals.
        // For simplicity I will demand to two separated functions the logic to handle the two cases.
        if (usd_normal_interp == TfToken("faceVarying") || usd_normal_interp == TfToken("uniform")) {
            // duplicated vertices between faces, each face's vertex has its own normal
            // * faceVarying: each vertex of each face has its own normal
            // * uniform: each vertex of each face has the same normal
            int number_of_triangles = current_face_vertex_count - 2;
            int usd_vertex_idx;
            int triangle_idx;
            int usd_normal_idx;
            int j;

            VtArray<int> current_normals_indices;
            if (usd_normal_interp == TfToken("faceVarying")) {
                current_normals_indices.resize(current_face_vertex_count);
                for (int i = 0; i < current_face_vertex_count; ++i) {
                    current_normals_indices[i] = vertex_offset + i;
                }

                if (reverse_winding) {
                    std::reverse(current_normals_indices.begin(), current_normals_indices.end());
                }
            } else {
                usd_normal_idx = usd_face_index;
            }

            for (triangle_idx = 0; triangle_idx < number_of_triangles; ++triangle_idx) {
                for (int i = 0; i < 3; ++i) {
                    if (i == 0) {
                        j = 0;
                        usd_vertex_idx = current_face_vertex_indices[0];
                    } else {
                        j = triangle_idx + i;
                    }

                    usd_vertex_idx = current_face_vertex_indices[j];
                    if (usd_normal_interp == TfToken("faceVarying"))
                        usd_normal_idx = current_normals_indices[j];

                    if (z_up){
                        gd_vertices.append(to_godot(ztoy_rot * usd_points[usd_vertex_idx]));
                        gd_normals.append(to_godot(ztoy_rot * usd_normals[usd_normal_idx]));
                    } else {
                        gd_vertices.append(to_godot(usd_points[usd_vertex_idx]));
                        gd_normals.append(to_godot(usd_normals[usd_normal_idx]));
                    }
                    gd_indices.append(gd_vertices.size() - 1);
                }
            }

        // make_face_varying_mesh(gd_vertices, gd_normals, gd_uvs, gd_colors, gd_indices);
        } else if (usd_normals.empty() || usd_normal_interp == TfToken("vertex") || usd_normal_interp == TfToken("varying") || usd_normal_interp == TfToken("constant")) {
            // shared vertices between adjacent faces
            // * vertex: each vertex has its own normal
            // * constant: one single normal for all the vertices
            int number_of_triangles = current_face_vertex_count - 2;
            int usd_vertex_idx;
            int triangle_idx;
            int usd_normal_idx;

            for (triangle_idx = 0; triangle_idx < number_of_triangles; ++triangle_idx) {
                int i = 0;
                if (triangle_idx > 0) {
                    i = 2;
                }
                for (; i < 3; ++i) {
                    if (triangle_idx > 0) {
                        gd_indices.append(gd_vertices.size() - (2 + triangle_idx));
                        gd_indices.append(gd_vertices.size() - 1);
                    }

                    usd_vertex_idx = current_face_vertex_indices[triangle_idx + i];
                    if (!usd_normals.empty()) {
                        if (usd_normal_interp == TfToken("constant")) {
                            usd_normal_idx = 0; // constant
                        } else {
                            usd_normal_idx = usd_vertex_idx;
                        }
                        if (z_up){
                            gd_normals.append(to_godot(ztoy_rot * usd_normals[usd_normal_idx]));
                        } else {
                            gd_normals.append(to_godot(usd_normals[usd_normal_idx]));
                        }
                    }

                    if (z_up){
                        gd_vertices.append(to_godot(ztoy_rot * usd_points[usd_vertex_idx]));
                    } else {
                        gd_vertices.append(to_godot(usd_points[usd_vertex_idx]));
                    }

                    gd_indices.append(gd_vertices.size() - 1);
                }
            }

        } else {
            print_error("USD Import: Unsupported normal interpolation");
            return nullptr;
        }

        vertex_offset += current_face_vertex_count;
    }

    Array arrays;
    arrays.resize(Mesh::ARRAY_MAX);
    arrays[Mesh::ARRAY_VERTEX] = gd_vertices;
    arrays[Mesh::ARRAY_INDEX]  = gd_indices;
    if (!gd_normals.is_empty())
        arrays[Mesh::ARRAY_NORMAL] = gd_normals;
    if (!gd_uvs.is_empty())
        arrays[Mesh::ARRAY_TEX_UV] = gd_uvs;
    if (!gd_colors.is_empty())
        arrays[Mesh::ARRAY_COLOR]  = gd_colors;

    Ref<ArrayMesh> array_mesh;
    array_mesh.instantiate();
    array_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);

    if (gd_normals.is_empty()) {
        Ref<SurfaceTool> st;
        st.instantiate();
        st->create_from(array_mesh, 0);
        st->generate_normals();
        array_mesh = st->commit();
    }

    MeshInstance3D *mesh_instance = memnew(MeshInstance3D);
    mesh_instance->set_mesh(array_mesh);
    set_node_transform(mesh_instance, usd_prim);
    return mesh_instance;
}

    // Ref<StandardMaterial3D> UsdMeshImportUtils::create_material(const UsdPrim &p_prim) {
    //     Ref<StandardMaterial3D> stdMaterial;
    //     // Check for displayColor attribute
    //     UsdPrim usdPrim(p_prim);
    //     if (usdPrim.HasAttribute(TfToken("primvars:displayColor"))) {
    //         // Get the display color
    //         UsdAttribute displayColorAttr = usdPrim.GetAttribute(TfToken("primvars:displayColor"));
    //         VtArray<GfVec3f> displayColors;
    //         if (displayColorAttr.Get(&displayColors) && !displayColors.empty()) {
    //             // Check for colorSpace metadata
    //             std::string colorSpace = "srgb";
    //             // In the example, colorSpace is a metadata on the displayColor attribute
    //             TfToken colorSpaceToken("colorSpace");
    //             if (displayColorAttr.HasMetadata(colorSpaceToken)) {
    //                 std::string colorSpaceValue;
    //                 displayColorAttr.GetMetadata(colorSpaceToken, &colorSpaceValue);
    //                 colorSpace = colorSpaceValue;
    //             }
    //             // Create a simple material
    //             Ref<Material> material;
    //             material.instantiate();

    //             // Set the albedo color from the displayColor
    //             GfVec3f color = displayColors[0];

    //             // Create a StandardMaterial3D
    //             stdMaterial.instantiate();

    //             // Set the albedo color from the displayColor
    //             Color godotColor(color[0], color[1], color[2]);
    //             stdMaterial->set_albedo(godotColor);
    //         }
    //     }
    //     return stdMaterial;
    // }


} // namespace godot