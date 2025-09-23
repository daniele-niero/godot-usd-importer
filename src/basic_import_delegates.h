#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <pxr/usd/usd/prim.h>



namespace godot_usd_importer {


void set_node_transform(godot::Node3D* gd_node, const pxr::UsdPrim &usd_prim);

godot::Node3D* import_xformable(const pxr::UsdPrim &usd_prim);


} // end godot namespace