#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <pxr/usd/usd/prim.h>


namespace godot_usd_importer {


godot::Node3D* import_cube(const pxr::UsdPrim &usd_prim);
godot::Node3D* import_sphere(const pxr::UsdPrim& usd_prim);
godot::Node3D* import_cylinder(const pxr::UsdPrim& usd_prim);
godot::Node3D* import_capsule(const pxr::UsdPrim& usd_prim);
godot::Node3D* import_cone(const pxr::UsdPrim& usd_prim);
void register_basic_solids();

godot::Node3D* import_mesh(const pxr::UsdPrim& usd_prim);


}