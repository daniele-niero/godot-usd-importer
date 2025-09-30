#include "mesh_import_delegates.h"
#include "basic_import_delegates.h"
#include "usd_visitor.h"
#include "mesh_utils.h"

#include <godot_cpp/classes/mesh_data_tool.hpp>
#include <godot_cpp/classes/surface_tool.hpp>
#include <godot_cpp/core/print_string.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/cylinder_mesh.hpp>
#include <godot_cpp/classes/capsule_mesh.hpp>

#include <pxr/base/gf/matrix3d.h>
#include <pxr/usd/usdGeom/primvar.h>
#include <pxr/usd/usdGeom/tokens.h>
#include <pxr/usd/usdGeom/metrics.h>
#include <pxr/usd/usdGeom/cube.h>
#include <pxr/usd/usdGeom/sphere.h>
#include <pxr/usd/usdGeom/cone.h>
#include <pxr/usd/usdGeom/cylinder.h>
#include <pxr/usd/usdGeom/capsule.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/usd/usdGeom/primvarsAPI.h>


PXR_NAMESPACE_USING_DIRECTIVE
using namespace godot;
namespace godot_usd_importer {

// ---- Standard Shapes ------------------------------------------------------------------


Node3D* import_cube(const UsdPrim& usd_prim, const godot::Dictionary &options) {
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


Node3D* import_sphere(const UsdPrim& usd_prim, const godot::Dictionary &options) {
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


Node3D* import_cylinder(const UsdPrim& usd_prim, const godot::Dictionary &options) {
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


Node3D* import_capsule(const UsdPrim& usd_prim, const godot::Dictionary &options) {
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


Node3D* import_cone(const UsdPrim& usd_prim, const godot::Dictionary &options) {
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

//  --------------------------------------------------------------------------------------


Node3D* import_mesh(const UsdPrim &usd_prim, const godot::Dictionary &options) {
    TfToken up_axis = UsdGeomGetStageUpAxis(usd_prim.GetStage());
    double meters_per_unit = UsdGeomGetStageMetersPerUnit(usd_prim.GetStage());
    unsigned int z_up = 0;
    bool convert_if_zup = options.get("zup_conversion", true);
    if (up_axis == UsdGeomTokens->z && convert_if_zup == true)
        z_up = 1;


    UsdGeomMesh usd_mesh(usd_prim);
    MeshData mesh_data(usd_mesh);
    if (!mesh_data.is_valid())
        return nullptr;

    // Arrays for the new Godot mesh
    GdMeshArrays gd_arrays;

    mesh_data.to_godot_arrays(gd_arrays, meters_per_unit, z_up);

    Array arrays;
    arrays.resize(Mesh::ARRAY_MAX);
    arrays[Mesh::ARRAY_VERTEX] = gd_arrays.vertices;
    arrays[Mesh::ARRAY_INDEX]  = gd_arrays.indices;
    if (!gd_arrays.normals.is_empty())
        arrays[Mesh::ARRAY_NORMAL] = gd_arrays.normals;
    if (!gd_arrays.uvs.is_empty())
        arrays[Mesh::ARRAY_TEX_UV] = gd_arrays.uvs;
    if (!gd_arrays.colors.is_empty())
        arrays[Mesh::ARRAY_COLOR]  = gd_arrays.colors;

    Ref<ArrayMesh> array_mesh;
    array_mesh.instantiate();
    array_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);

    if (gd_arrays.normals.is_empty()) {
        Ref<SurfaceTool> st;
        st.instantiate();
        st->create_from(array_mesh, 0);
        st->generate_normals();
        array_mesh = st->commit();
    }

    MeshInstance3D *mesh_instance = memnew(MeshInstance3D);
    mesh_instance->set_mesh(array_mesh);
    set_node_transform(mesh_instance, usd_prim, convert_if_zup);
    return mesh_instance;
}




// Node3D* import_mesh(const UsdPrim &usd_prim, const godot::Dictionary &options) {
//     TfToken up_axis = UsdGeomGetStageUpAxis(usd_prim.GetStage());
//     double meters_per_unit = UsdGeomGetStageMetersPerUnit(usd_prim.GetStage());
//     unsigned int z_up = 0;
//     bool convert_if_zup = options.get("zup_conversion", true);
//     if (up_axis == UsdGeomTokens->z && convert_if_zup == true)
//         z_up = 1;


//     UsdGeomMesh usd_mesh(usd_prim);
//     MeshData mesh_data(usd_mesh);
//     if (!mesh_data.is_valid())
//         return nullptr;

//     bool reverse_winding = mesh_data.reverse_winding();




//     // Arrays for the new Godot mesh
//     PackedVector3Array gd_vertices;
//     PackedVector3Array gd_normals;
//     PackedVector2Array gd_uvs;
//     PackedColorArray gd_colors;
//     PackedInt32Array gd_indices;

//     int vertex_offset = 0;

//     for (size_t usd_face_index = 0; usd_face_index < mesh_data.usd_face_vertex_counts.size(); ++usd_face_index) {
//         int current_face_vertex_count = mesh_data.usd_face_vertex_counts[usd_face_index];

//         VtArray<int> current_face_vertex_indices;
//         current_face_vertex_indices.resize(current_face_vertex_count);
//         for (int i = 0; i < current_face_vertex_count; ++i) {
//             current_face_vertex_indices[i] = mesh_data.usd_face_vertex_indices[vertex_offset + i];
//         }

//         if (reverse_winding) {
//             std::reverse(current_face_vertex_indices.begin(), current_face_vertex_indices.end());
//         }

//         int number_of_triangles = current_face_vertex_count - 2;

//         // Godot can only have flat or smooth normals.
//         // When smooth (for the whole mesh), we can have shared vertex between faces, and normals will be one per vertex.
//         // We also use an indices array to define the point used by a face.
//         // When flat normals, we need to duplicate vertices so each face has its own vertices and normals.
//         if (mesh_data.get_vertex_strategy() == MeshData::GodotVertexStrategy::Split) {
//             // duplicated vertices between faces, each face's vertex has its own normal
//             // * faceVarying: each vertex of each face has its own normal
//             // * uniform: each vertex of each face has the same normal
//             int usd_vertex_idx;
//             int triangle_idx;
//             int usd_normal_idx;
//             int j;

//             VtArray<int> current_normals_indices;
//             if (mesh_data.usd_normal_interp == UsdGeomTokens->faceVarying) {
//                 current_normals_indices.resize(current_face_vertex_count);
//                 for (int i = 0; i < current_face_vertex_count; ++i) {
//                     current_normals_indices[i] = vertex_offset + i;
//                 }

//                 if (reverse_winding) {
//                     std::reverse(current_normals_indices.begin(), current_normals_indices.end());
//                 }
//             } else if (mesh_data.usd_normal_interp == UsdGeomTokens->uniform) {
//                 usd_normal_idx = usd_face_index;
//             }

//             for (triangle_idx = 0; triangle_idx < number_of_triangles; ++triangle_idx) {
//                 for (int i = 0; i < 3; ++i) {
//                     if (i == 0) {
//                         j = 0;
//                         usd_vertex_idx = current_face_vertex_indices[0];
//                     } else {
//                         j = triangle_idx + i;
//                     }

//                     usd_vertex_idx = current_face_vertex_indices[j];
//                     if (mesh_data.usd_normal_interp == UsdGeomTokens->faceVarying)
//                         usd_normal_idx = current_normals_indices[j];

//                     if (z_up){
//                         gd_vertices.append(to_godot(ztoy_rot * mesh_data.usd_points[usd_vertex_idx]) * meters_per_unit);
//                         gd_normals.append(to_godot(ztoy_rot * mesh_data.usd_normals[usd_normal_idx]) * meters_per_unit);
//                     } else {
//                         gd_vertices.append(to_godot(mesh_data.usd_points[usd_vertex_idx]) * meters_per_unit);
//                         gd_normals.append(to_godot(mesh_data.usd_normals[usd_normal_idx]) * meters_per_unit);
//                     }
//                     gd_indices.append(gd_vertices.size() - 1);
//                 }
//             }
//         } else if (usd_normals.empty() || usd_normal_interp == UsdGeomTokens->vertex || usd_normal_interp == UsdGeomTokens->varying || usd_normal_interp == UsdGeomTokens->constant) {
//             // shared vertices between adjacent faces
//             // * vertex: each vertex has its own normal
//             // * constant: one single normal for all the vertices
//             int usd_vertex_idx;
//             int triangle_idx;
//             int usd_normal_idx;

//             for (triangle_idx = 0; triangle_idx < number_of_triangles; ++triangle_idx) {
//                 int i = 0;
//                 if (triangle_idx > 0) {
//                     i = 2;
//                 }
//                 for (; i < 3; ++i) {
//                     if (triangle_idx > 0) {
//                         gd_indices.append(gd_vertices.size() - (2 + triangle_idx));
//                         gd_indices.append(gd_vertices.size() - 1);
//                     }

//                     usd_vertex_idx = current_face_vertex_indices[triangle_idx + i];
//                     if (!usd_normals.empty()) {
//                         if (usd_normal_interp == UsdGeomTokens->constant) {
//                             usd_normal_idx = 0; // constant
//                         } else {
//                             usd_normal_idx = usd_vertex_idx;
//                         }
//                         if (z_up){
//                             gd_normals.append(to_godot(ztoy_rot * usd_normals[usd_normal_idx]) * meters_per_unit);
//                         } else {
//                             gd_normals.append(to_godot(usd_normals[usd_normal_idx]) * meters_per_unit);
//                         }
//                     }

//                     if (z_up){
//                         gd_vertices.append(to_godot(ztoy_rot * usd_points[usd_vertex_idx]) * meters_per_unit);
//                     } else {
//                         gd_vertices.append(to_godot(usd_points[usd_vertex_idx]) * meters_per_unit);
//                     }

//                     gd_indices.append(gd_vertices.size() - 1);
//                 }
//             }

//         } else {
//             print_error("USD Import: Unsupported normal interpolation");
//             return nullptr;
//         }

//         vertex_offset += current_face_vertex_count;
//     }

//     Array arrays;
//     arrays.resize(Mesh::ARRAY_MAX);
//     arrays[Mesh::ARRAY_VERTEX] = gd_vertices;
//     arrays[Mesh::ARRAY_INDEX]  = gd_indices;
//     if (!gd_normals.is_empty())
//         arrays[Mesh::ARRAY_NORMAL] = gd_normals;
//     if (!gd_uvs.is_empty())
//         arrays[Mesh::ARRAY_TEX_UV] = gd_uvs;
//     if (!gd_colors.is_empty())
//         arrays[Mesh::ARRAY_COLOR]  = gd_colors;

//     Ref<ArrayMesh> array_mesh;
//     array_mesh.instantiate();
//     array_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);

//     if (gd_normals.is_empty()) {
//         Ref<SurfaceTool> st;
//         st.instantiate();
//         st->create_from(array_mesh, 0);
//         st->generate_normals();
//         array_mesh = st->commit();
//     }

//     MeshInstance3D *mesh_instance = memnew(MeshInstance3D);
//     mesh_instance->set_mesh(array_mesh);
//     set_node_transform(mesh_instance, usd_prim, convert_if_zup);
//     return mesh_instance;
// }



} // namespace godot