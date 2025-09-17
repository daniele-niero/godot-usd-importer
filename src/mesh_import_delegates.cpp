#include "mesh_import_delegates.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include <pxr/usd/usdGeom/primvarsAPI.h>

namespace godot {

Node3D* import_cube(const UsdPrim& usd_prim, Node3D* godot_parent) {
    // Create a new BoxMesh
    Ref<BoxMesh> box_mesh;
    box_mesh.instantiate();

    UsdGeomCube usd_cube(usd_prim);
    double size = 2.0; // Default size (2.0 because UsdGeomCube size is the half-extent)
    if (usd_cube) {
        UsdAttribute sizeAttr = usd_cube.GetSizeAttr();
        if (sizeAttr) {
            sizeAttr.Get(&size);
        }
    }

    box_mesh->set_size(Vector3(size, size, size));

    MeshInstance3D *mesh_instance = memnew(MeshInstance3D);
    mesh_instance->set_mesh(box_mesh);
    godot_parent->add_child(mesh_instance);
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

    // Ref<SphereMesh> UsdMeshImportUtils::import_sphere(const pxr::UsdGeomSphere& p_sphere) {
    //     // Create a new SphereMesh
    //     Ref<SphereMesh> sphere_mesh;
    //     sphere_mesh.instantiate();

    //     // Get the radius from the USD sphere
    //     double radius = 1.0;
    //     p_sphere.GetRadiusAttr().Get(&radius);

    //     // Set the radius
    //     sphere_mesh->set_radius(radius);
    //     sphere_mesh->set_height(radius * 2.0); // Height is diameter in Godot

    //     //UtilityFunctions::print("USD Import: Imported sphere with radius ", radius);
    //     return sphere_mesh;
    // }

    // Ref<CylinderMesh> UsdMeshImportUtils::import_cylinder(const pxr::UsdGeomCylinder& p_cylinder) {
    //     // Create a new CylinderMesh
    //     Ref<CylinderMesh> cylinder_mesh;
    //     cylinder_mesh.instantiate();

    //     // Get the radius and height from the USD cylinder
    //     double radius = 1.0;
    //     double height = 2.0;
    //     p_cylinder.GetRadiusAttr().Get(&radius);
    //     p_cylinder.GetHeightAttr().Get(&height);

    //     // Set the radius and height
    //     cylinder_mesh->set_top_radius(radius);
    //     cylinder_mesh->set_bottom_radius(radius);
    //     cylinder_mesh->set_height(height);

    //     //UtilityFunctions::print("USD Import: Imported cylinder with radius ", radius, " and height ", height);
    //     return cylinder_mesh;
    // }

    // Ref<CylinderMesh> UsdMeshImportUtils::import_cone(const pxr::UsdGeomCone& p_cone) {
    //     // Create a new CylinderMesh (Godot uses CylinderMesh for cones too)
    //     Ref<CylinderMesh> cone_mesh;
    //     cone_mesh.instantiate();

    //     // Get the radius and height from the USD cone
    //     double radius = 1.0;
    //     double height = 2.0;
    //     p_cone.GetRadiusAttr().Get(&radius);
    //     p_cone.GetHeightAttr().Get(&height);

    //     // Set the radius and height (top radius = 0 for a cone)
    //     cone_mesh->set_top_radius(0.0);
    //     cone_mesh->set_bottom_radius(radius);
    //     cone_mesh->set_height(height);

    //     //UtilityFunctions::print("USD Import: Imported cone with radius ", radius, " and height ", height);
    //     return cone_mesh;
    // }

    // Ref<CapsuleMesh> UsdMeshImportUtils::import_capsule(const pxr::UsdGeomCapsule& p_capsule) {
    //     // Create a new CapsuleMesh
    //     Ref<CapsuleMesh> capsule_mesh;
    //     capsule_mesh.instantiate();

    //     // Get the radius and height from the USD capsule
    //     double radius = 1.0;
    //     double height = 2.0;
    //     p_capsule.GetRadiusAttr().Get(&radius);
    //     p_capsule.GetHeightAttr().Get(&height);

    //     // Set the radius and height
    //     capsule_mesh->set_radius(radius);
    //     capsule_mesh->set_height(height);

    //     //UtilityFunctions::print("USD Import: Imported capsule with radius ", radius, " and height ", height);
    //     return capsule_mesh;
    // }

    // // Helper to get primvars as arrays of the data they contains

    // static VtArray<GfVec3f> get_primvar_vec3f_array(const UsdGeomPrimvar& primvar) {
    //     VtArray<GfVec3f> array;
    //     primvar.Get(&array);
    //     return array;
    // }

    // static VtArray<GfVec2f> get_primvar_vec2f_array(const UsdGeomPrimvar& primvar) {
    //     VtArray<GfVec2f> array;
    //     primvar.Get(&array);
    //     return array;
    // }

    // static VtArray<GfVec4f> get_primvar_color_array(const UsdGeomPrimvar& primvar) {
    //     VtArray<GfVec4f> array4;
    //     if (primvar.Get(&array4) && !array4.empty()) {
    //         return array4;
    //     }
    //     VtArray<GfVec3f> array3;
    //     if (primvar.Get(&array3) && !array3.empty()) {
    //         VtArray<GfVec4f> result;
    //         result.resize(array3.size());
    //         for (size_t i = 0; i < array3.size(); ++i) {
    //             const GfVec3f& v = array3[i];
    //             result[i] = GfVec4f(v[0], v[1], v[2], 1.0f);
    //         }
    //         return result;
    //     }
    //     return VtArray<GfVec4f>();
    // }

    // static Vector3 to_godot(const GfVec3f& usd_vector) {
    //     return Vector3(usd_vector[0], usd_vector[1], usd_vector[2]);
    // }

    // static Vector2 to_godot(const GfVec2f& usd_vector) {
    //     return Vector2(usd_vector[0], usd_vector[1]);
    // }

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

    // // Helper to extract all relevant Usd mesh data
    // static bool get_usd_mesh_data(
    //     const UsdGeomMesh& usd_mesh,
    //     VtArray<GfVec3f>& usd_points,
    //     VtArray<GfVec3f>& usd_normals,
    //     VtArray<GfVec2f>& usd_uv_map,
    //     VtArray<int>& usd_face_vertex_counts,
    //     VtArray<int>& usd_face_vertex_indices,
    //     TfToken& usd_face_orientation_token,
    //     UsdGeomPrimvar& usd_uv_primvar,
    //     TfToken& usd_normal_interp,
    //     TfToken& usd_uv_interp
    // ) {
    //     if (!usd_mesh.GetPointsAttr().Get(&usd_points) ||
    //         !usd_mesh.GetNormalsAttr().Get(&usd_normals) ||
    //         !usd_mesh.GetFaceVertexCountsAttr().Get(&usd_face_vertex_counts) ||
    //         !usd_mesh.GetFaceVertexIndicesAttr().Get(&usd_face_vertex_indices))
    //     {
    //         print_error("USD Import: Failed to get essential mesh attributes");
    //         return false;
    //     }

    //     usd_mesh.GetOrientationAttr().Get(&usd_face_orientation_token);

    //     const UsdGeomPrimvarsAPI primvars(usd_mesh);
    //     usd_uv_primvar = UsdGeomPrimvar();
    //     std::vector<UsdGeomPrimvar> primvar_list = primvars.GetPrimvars();
    //     for (const auto& pv : primvar_list) {
    //         TfToken primvar_name = pv.GetBaseName();
    //         // print_line("USD Import: Found primvar ", primvar_name);
    //         if (!usd_uv_primvar && (primvar_name == TfToken("st") || primvar_name == TfToken("uv") || primvar_name == TfToken("UVMap") || pv.GetTypeName() == SdfValueTypeNames->TexCoord2fArray)) {
    //             usd_uv_primvar = pv;
    //         }
    //         if (usd_uv_primvar) break;
    //     }

    //     usd_normal_interp = usd_mesh.GetNormalsInterpolation();
    //     usd_uv_interp = usd_uv_primvar.IsDefined() ? usd_uv_primvar.GetInterpolation() : UsdGeomTokens->vertex;
    //     usd_uv_map = get_primvar_vec2f_array(usd_uv_primvar);

    //     return true;
    // }


    // static bool make_face_varying_mesh(
    //     PackedVector3Array& gd_vertices,
    //     PackedVector3Array& gd_normals,
    //     PackedVector2Array& gd_uvs,
    //     PackedColorArray& gd_colors,
    //     PackedInt32Array& gd_indices
    // ) {
    //     return true;
    // }

    // static bool make_full_smoot_mesh(
    //     PackedVector3Array& gd_vertices,
    //     PackedVector3Array& gd_normals,
    //     PackedVector2Array& gd_uvs,
    //     PackedColorArray& gd_colors,
    //     PackedInt32Array& gd_indices
    // ) {
    //     return true;
    // }


    // Ref<Mesh> UsdMeshImportUtils::import_geom_mesh(const UsdGeomMesh& usd_mesh) {
    //     // variables that will hold the data from the USD mesh
    //     VtArray<GfVec3f> usd_points;
    //     VtArray<GfVec3f> usd_normals;
    //     VtArray<GfVec2f> usd_uv_map;
    //     VtArray<int> usd_face_vertex_counts;
    //     VtArray<int> usd_face_vertex_indices;
    //     TfToken usd_face_orientation_token;
    //     UsdGeomPrimvar usd_uv_primvar;
    //     TfToken usd_normal_interp;
    //     TfToken usd_uv_interp;

    //     // get data from the USD mesh
    //     if (!get_usd_mesh_data(usd_mesh, usd_points, usd_normals, usd_uv_map, usd_face_vertex_counts, usd_face_vertex_indices, usd_face_orientation_token, usd_uv_primvar, usd_normal_interp, usd_uv_interp)) {
    //         print_error("USD Import: couldn't generate ArrayMesh from USD mesh data");
    //         return Ref<ArrayMesh>();
    //     }

    //     bool reverse_winding = (usd_face_orientation_token != TfToken("leftHanded"));

    //     // Arrays for the new Godot mesh
    //     PackedVector3Array gd_vertices;
    //     PackedVector3Array gd_normals;
    //     PackedVector2Array gd_uvs;
    //     PackedColorArray gd_colors;
    //     PackedInt32Array gd_indices;

    //     int triangle_index = 0;
    //     int vertex_offset = 0;

    //     for (int usd_face_index = 0; usd_face_index < usd_face_vertex_counts.size(); ++usd_face_index) {
    //         int current_face_vertex_count = usd_face_vertex_counts[usd_face_index];

    //         VtArray<int> current_face_vertex_indices;
    //         current_face_vertex_indices.resize(current_face_vertex_count);
    //         for (int i = 0; i < current_face_vertex_count; ++i) {
    //             current_face_vertex_indices[i] = usd_face_vertex_indices[vertex_offset + i];
    //         }

    //         if (reverse_winding) {
    //             std::reverse(current_face_vertex_indices.begin(), current_face_vertex_indices.end());
    //         }

    //         print_line("Reverse winding: ", reverse_winding);
    //         print_line("Vertex offset: ", vertex_offset);
    //         print_line("Processing face ", usd_face_index, " with ", current_face_vertex_count, " vertices");

    //         // Godot can only have flat or smooth normals.
    //         // When smooth (for the whole mesh), we can have shared vertex between faces and normals will be one per vertex.
    //         // We also use an indices array to define the point used by a face.
    //         // When flat normals, we need to duplicate vertices so each face has its own vertices and normals.
    //         // For semplicity I will demand to two separated functions the logic to handle the two cases.
    //         if (usd_normal_interp == TfToken("faceVarying") || usd_normal_interp == TfToken("uniform")) {
    //             // duplicated vertices between faces, each face's vertex has its own normal
    //             // * faceVarying: each vertex of each face has its own normal
    //             // * uniform: each vertex of each face has the same normal
    //             int number_of_triangles = current_face_vertex_count - 2;
    //             int usd_vertex_idx;
    //             int normal_idx;
    //             int triangle_idx;
    //             int usd_normal_idx;
    //             int j;

    //             VtArray<int> current_normals_indices;
    //             if (usd_normal_interp == TfToken("faceVarying")) {
    //                 current_normals_indices.resize(current_face_vertex_count);
    //                 for (int i = 0; i < current_face_vertex_count; ++i) {
    //                     current_normals_indices[i] = vertex_offset + i;
    //                 }

    //                 if (reverse_winding) {
    //                     std::reverse(current_normals_indices.begin(), current_normals_indices.end());
    //                 }
    //             } else {
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
    //                     if (usd_normal_interp == TfToken("faceVarying"))
    //                         usd_normal_idx = current_normals_indices[j];

    //                     gd_vertices.append(to_godot(usd_points[usd_vertex_idx]));
    //                     gd_indices.append(gd_vertices.size() - 1);

    //                     gd_normals.append(to_godot(usd_normals[usd_normal_idx]));

    //                     print_line("  Vertex ", i, ": point index ", usd_vertex_idx);
    //                     print_line("  Normal index: ", usd_normal_idx);
    //                 }
    //             }

    //             // make_face_varying_mesh(gd_vertices, gd_normals, gd_uvs, gd_colors, gd_indices);
    //         } else if (usd_normal_interp == TfToken("vertex") || usd_normal_interp == TfToken("varying") || usd_normal_interp == TfToken("constant")) {
    //             // duplicated vertices between faces
    //             // * vertex: each vertex has its own normal
    //             // * constatnt: one single normal for all the vertices
    //             int number_of_triangles = current_face_vertex_count - 2;
    //             int usd_vertex_idx;
    //             int normal_idx;
    //             int triangle_idx;
    //             int usd_normal_idx;

    //             for (triangle_idx = 0; triangle_idx < number_of_triangles; ++triangle_idx) {
    //                 int i = 0;
    //                 if (triangle_idx > 0) {
    //                     i = 2;
    //                 }
    //                 for (i; i < 3; ++i) {
    //                     print_line("Triangle idx: ", triangle_idx, " i: ", i);
    //                     if (triangle_idx > 0) {
    //                         gd_indices.append(gd_vertices.size() - (2 + triangle_idx));
    //                         gd_indices.append(gd_vertices.size() - 1);
    //                     }

    //                     usd_vertex_idx = current_face_vertex_indices[triangle_idx + i];
    //                     if (usd_normal_interp == TfToken("constant"))
    //                         usd_normal_idx = 0; // constant
    //                     else
    //                         usd_normal_idx = usd_vertex_idx;

    //                     gd_vertices.append(to_godot(usd_points[usd_vertex_idx]));
    //                     gd_indices.append(gd_vertices.size() - 1);

    //                     gd_normals.append(to_godot(usd_normals[usd_normal_idx]));

    //                     print_line("  Vertex ", i, ": point index ", usd_vertex_idx);
    //                     print_line("  Normal index: ", usd_normal_idx);
    //                 }
    //             }



    //         } else {
    //             print_error("USD Import: Unsupported normal interpolation");
    //             return Ref<ArrayMesh>();
    //         }

    //         vertex_offset += current_face_vertex_count;
    //     }

    //     //     int usd_vertex_idx;
    //     //     int normal_idx;
    //     //     int triangle_idx = 0;

    //     //     // first triangle for this face
    //     //     for (int i = 0; i < 3; ++i) {
    //     //         usd_vertex_idx = current_face_vertex_indices[i];
    //     //         gd_vertices.append(to_godot(usd_points[usd_vertex_idx]));
    //     //         gd_indices.append(gd_vertices.size() - 1);

    //     //         normal_idx = get_indx_of_interpolated_attribute(usd_normal_interp, usd_face_index, usd_vertex_idx, usd_vertex_idx);
    //     //         gd_normals.append(to_godot(usd_normals[normal_idx]));


    //     //         print_line("  Vertex ", i, ": point index ", usd_vertex_idx);
    //     //         print_line("    Normal index: ", normal_idx);
    //     //     }

    //     //     int number_of_triangles = current_face_vertex_count - 2;
    //     //     // if more than 3 vertices, create the other triangles simply adding a new vertex each time
    //     //     // and reusing the first two vertices/indices of the previous triangle
    //     //     for (triangle_idx = 1; triangle_idx < number_of_triangles; ++triangle_idx) {
    //     //         gd_indices.append(gd_vertices.size() - (2 + triangle_idx));
    //     //         gd_indices.append(gd_vertices.size() - 1);

    //     //         Vector3 normal0 = gd_normals[gd_normals.size() - (2 + triangle_idx)];
    //     //         Vector3 normal1 = gd_normals[gd_normals.size() - 1];
    //     //         gd_normals.append(normal0);
    //     //         gd_normals.append(normal1);


    //     //         usd_vertex_idx = current_face_vertex_indices[triangle_idx + 2];
    //     //         gd_vertices.append(to_godot(usd_points[usd_vertex_idx]));
    //     //         gd_indices.append(gd_vertices.size() - 1);

    //     //         normal_idx = get_indx_of_interpolated_attribute(usd_normal_interp, usd_face_index, usd_vertex_idx, usd_vertex_idx);
    //     //         gd_normals.append(to_godot(usd_normals[normal_idx]));

    //     //         print_line("  Vertex ", triangle_idx + 3, ": point index ", usd_vertex_idx);
    //     //         print_line("    Normal index: ", normal_idx);
    //     //     }

    //     //     vertex_offset += current_face_vertex_count;
    //     // }

    //     //print size of gd_points and gd_normals and gd_indices
    //     print_line("Imported ", gd_vertices.size(), " vertices");
    //     print_line("Imported ", gd_normals.size(), " normals");
    //     print_line("Imported ", gd_indices.size(), " indices");

    //     // print gd normals one by one
    //     for (int i = 0; i < gd_normals.size(); ++i) {
    //         print_line("  Normal ", i, ": ", gd_normals[i]);
    //     }


    //     // if (!reverse_winding) {
    //     //     print_line("No reverse winding, inverting normals");
    //     //     // invert normals one by one
    //     //     for (int i = 0; i < gd_normals.size(); ++i) {
    //     //         gd_normals[i] = gd_normals[i].inverse();
    //     //     }
    //     // }

    //     // // Synthesize weighted normals if original normals were missing
    //     // bool had_normals = false;
    //     // for (int i = 0; i < gd_normals.size(); ++i) {
    //     //     if (gd_normals[i].length_squared() > 1e-5f) {
    //     //         had_normals = true;
    //     //         break;
    //     //     }
    //     // }

    //     // if (!had_normals) {
    //     //     gd_normals.resize(gd_vertices.size());
    //     //     for (int i = 0; i < gd_normals.size(); ++i)
    //     //         gd_normals[i] = Vector3(0, 0, 0);

    //     //     for (int i = 0; i < gd_indices.size(); i += 3) {
    //     //         int i0 = gd_indices[i];
    //     //         int i1 = gd_indices[i + 1];
    //     //         int i2 = gd_indices[i + 2];

    //     //         Vector3 v0 = gd_vertices[i0];
    //     //         Vector3 v1 = gd_vertices[i1];
    //     //         Vector3 v2 = gd_vertices[i2];

    //     //         Vector3 e1 = v1 - v0;
    //     //         Vector3 e2 = v2 - v0;
    //     //         Vector3 face_normal = e1.cross(e2);
    //     //         float area = face_normal.length() * 0.5f;
    //     //         face_normal = face_normal.normalized();

    //     //         gd_normals[i0] += face_normal * area;
    //     //         gd_normals[i1] += face_normal * area;
    //     //         gd_normals[i2] += face_normal * area;
    //     //     }

    //     //     for (int i = 0; i < gd_normals.size(); ++i)
    //     //         gd_normals[i] = gd_normals[i].normalized();
    //     // }

    //     // Ensure consistent attribute sizes
    //     int vertex_count = gd_vertices.size();

    //     if (gd_uvs.size() != vertex_count) {
    //         gd_uvs.resize(vertex_count);
    //         for (int i = 0; i < vertex_count; ++i)
    //             gd_uvs[i] = Vector2();
    //     }

    //     if (gd_colors.size() != vertex_count) {
    //         gd_colors.resize(vertex_count);
    //         for (int i = 0; i < vertex_count; ++i)
    //             gd_colors[i] = Color(1, 1, 1, 1);
    //     }

    //     Array arrays;
    //     arrays.resize(Mesh::ARRAY_MAX);
    //     arrays[Mesh::ARRAY_VERTEX] = gd_vertices;
    //     arrays[Mesh::ARRAY_NORMAL] = gd_normals;
    //     arrays[Mesh::ARRAY_TEX_UV] = gd_uvs;
    //     arrays[Mesh::ARRAY_COLOR] = gd_colors;
    //     arrays[Mesh::ARRAY_INDEX] = gd_indices;

    //     Ref<ArrayMesh> array_mesh = memnew(ArrayMesh);
    //     array_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);
    //     return array_mesh;
    // }

} // namespace godot