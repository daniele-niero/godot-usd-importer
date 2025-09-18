#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <pxr/usd/usd/prim.h>


PXR_NAMESPACE_USING_DIRECTIVE
namespace godot {


void set_node_transform(Node3D* gd_node, const UsdPrim &usd_prim);

Node3D* import_xformable(const UsdPrim &usd_prim);


} // end godot namespace