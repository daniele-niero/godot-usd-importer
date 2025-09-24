#pragma once

#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "godot_cpp/variant/vector3.hpp"
#include "pxr/base/gf/matrix4d.h"
#include "pxr/base/tf/token.h"
#include "pxr/usd/usdGeom/tokens.h"
#include <pxr/usd/usdGeom/primvarsAPI.h>
#include <pxr/usd/usdGeom/xformCommonAPI.h>


namespace godot_usd_importer {

const double cDegToRad = M_PI / 180.0;

pxr::GfMatrix4d ZupToYup(pxr::GfMatrix4d &in_mat);

pxr::VtArray<pxr::GfVec3f> get_primvar_vec3f_array(const pxr::UsdGeomPrimvar& primvar);
pxr::VtArray<pxr::GfVec2f> get_primvar_vec2f_array(const pxr::UsdGeomPrimvar& primvar);
pxr::VtArray<pxr::GfVec4f> get_primvar_color_array(const pxr::UsdGeomPrimvar& primvar);

godot::Vector3 to_godot(const pxr::GfVec3f &usd_vector);
godot::Vector2 to_godot(const pxr::GfVec2f &usd_vector);
godot::Vector3 to_godot(const pxr::GfVec3d &usd_vector);
godot::Vector2 to_godot(const pxr::GfVec2d &usd_vector);
godot::EulerOrder to_godot(const pxr::UsdGeomXformCommonAPI::RotationOrder rotOrder);

godot::EulerOrder get_rotation_order(const pxr::UsdGeomXformable &xformable);
}
