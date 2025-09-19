#pragma once
#include "godot_cpp/variant/vector2.hpp"
#include "godot_cpp/variant/vector3.hpp"
#include <pxr/usd/usdGeom/primvarsAPI.h>


PXR_NAMESPACE_USING_DIRECTIVE
namespace godot {

VtArray<GfVec3f> get_primvar_vec3f_array(const UsdGeomPrimvar& primvar);
VtArray<GfVec2f> get_primvar_vec2f_array(const UsdGeomPrimvar& primvar);
VtArray<GfVec4f> get_primvar_color_array(const UsdGeomPrimvar& primvar);

Vector3 to_godot(const GfVec3f& usd_vector);
Vector2 to_godot(const GfVec2f& usd_vector);

}
