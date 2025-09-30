#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <pxr/usd/usd/prim.h>



namespace godot_usd_importer {


void set_node_transform(godot::Node3D* gd_node, const pxr::UsdPrim &usd_prim, const bool convert_if_zup=true);

godot::Node3D* import_xformable(const pxr::UsdPrim &usd_prim, const godot::Dictionary &options);


} // end godot namespace