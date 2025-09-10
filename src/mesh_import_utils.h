#pragma once

#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/primitive_mesh.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/cylinder_mesh.hpp>
#include <godot_cpp/classes/capsule_mesh.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

// USD headers
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usdGeom/gprim.h>
#include <pxr/usd/usdGeom/cube.h>
#include <pxr/usd/usdGeom/sphere.h>
#include <pxr/usd/usdGeom/cone.h>
#include <pxr/usd/usdGeom/cylinder.h>
#include <pxr/usd/usdGeom/capsule.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/base/gf/vec3f.h>

PXR_NAMESPACE_USING_DIRECTIVE


namespace godot {

struct UsdMeshImportUtils {
    // Import a USD mesh prim into a Godot mesh, delegates to the
    // appropriate import method based on the prim type
    static Ref<Mesh> import_mesh_from_prim(const pxr::UsdPrim& p_prim);

    static Ref<BoxMesh> import_cube(const pxr::UsdPrim &p_prim);
    static Ref<SphereMesh> import_sphere(const pxr::UsdGeomSphere& p_sphere);
    static Ref<CylinderMesh> import_cylinder(const pxr::UsdGeomCylinder& p_cylinder);
    static Ref<CylinderMesh> import_cone(const pxr::UsdGeomCone& p_cone);
    static Ref<CapsuleMesh> import_capsule(const pxr::UsdGeomCapsule& p_capsule);
    static Ref<Mesh> import_geom_mesh(const pxr::UsdGeomMesh &p_mesh);
    static Ref<StandardMaterial3D> create_material(const pxr::UsdPrim& p_prim);
};
}