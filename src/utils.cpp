#include "utils.h"

namespace godot {

VtArray<GfVec3f> get_primvar_vec3f_array(const UsdGeomPrimvar& primvar) {
    VtArray<GfVec3f> array;
    primvar.Get(&array);
    return array;
}

VtArray<GfVec2f> get_primvar_vec2f_array(const UsdGeomPrimvar& primvar) {
    VtArray<GfVec2f> array;
    primvar.Get(&array);
    return array;
}

VtArray<GfVec4f> get_primvar_color_array(const UsdGeomPrimvar& primvar) {
    VtArray<GfVec4f> array4;
    if (primvar.Get(&array4) && !array4.empty()) {
        return array4;
    }
    VtArray<GfVec3f> array3;
    if (primvar.Get(&array3) && !array3.empty()) {
        VtArray<GfVec4f> result;
        result.resize(array3.size());
        for (size_t i = 0; i < array3.size(); ++i) {
            const GfVec3f& v = array3[i];
            result[i] = GfVec4f(v[0], v[1], v[2], 1.0f);
        }
        return result;
    }
    return VtArray<GfVec4f>();
}


Vector3 to_godot(const GfVec3f& usd_vector) {
    return Vector3(usd_vector[0], usd_vector[1], usd_vector[2]);
}

Vector2 to_godot(const GfVec2f& usd_vector) {
    return Vector2(usd_vector[0], usd_vector[1]);
}

}