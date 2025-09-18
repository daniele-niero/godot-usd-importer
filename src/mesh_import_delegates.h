#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <pxr/usd/usd/prim.h>


PXR_NAMESPACE_USING_DIRECTIVE
namespace godot {


Node3D* import_cube(const UsdPrim &usd_prim);
Node3D* import_sphere(const UsdPrim& usd_prim);
Node3D* import_cylinder(const UsdPrim& usd_prim);
Node3D* import_capsule(const UsdPrim& usd_prim);
Node3D* import_cone(const UsdPrim& usd_prim);
void register_basic_solids();

Node3D* import_mesh(const UsdPrim& usd_prim);


}