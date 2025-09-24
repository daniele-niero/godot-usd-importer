#include "utils.h"

#include "godot_cpp/classes/global_constants.hpp"
#include "pxr/base/gf/matrix4d.h"


PXR_NAMESPACE_USING_DIRECTIVE
using namespace godot;
namespace godot_usd_importer {

static const GfMatrix3d ztoy_rot_inv = ztoy_rot.GetInverse();
static const GfMatrix4d ztoy_mat_inv = ztoy_mat.GetInverse();


GfMatrix4d ZupToYup(GfMatrix4d &in_mat) {
    return ztoy_mat * in_mat * ztoy_mat_inv;
}

GfMatrix3d ZupToYup(GfMatrix3d &in_rot) {
    return ztoy_rot * in_rot * ztoy_rot_inv;
}


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

Vector3 to_godot(const GfVec3d& usd_vector) {
    return Vector3(usd_vector[0], usd_vector[1], usd_vector[2]);
}

Vector2 to_godot(const GfVec2d& usd_vector) {
    return Vector2(usd_vector[0], usd_vector[1]);
}


EulerOrder to_godot(const UsdGeomXformCommonAPI::RotationOrder usd_rot_order) {
    switch (usd_rot_order) {
        case UsdGeomXformCommonAPI::RotationOrder::RotationOrderXYZ:
            return EulerOrder::EULER_ORDER_XYZ;
        case UsdGeomXformCommonAPI::RotationOrderXZY:
            return EulerOrder::EULER_ORDER_XZY;
        case UsdGeomXformCommonAPI::RotationOrderYXZ:
            return EulerOrder::EULER_ORDER_YXZ;
        case UsdGeomXformCommonAPI::RotationOrderYZX:
            return EulerOrder::EULER_ORDER_YZX;
        case UsdGeomXformCommonAPI::RotationOrderZXY:
            return EulerOrder::EULER_ORDER_ZXY;
        case UsdGeomXformCommonAPI::RotationOrderZYX:
            return EulerOrder::EULER_ORDER_ZYX;
        default:
            return EulerOrder::EULER_ORDER_XYZ;
    }
}

EulerOrder get_rotation_order(const UsdGeomXformable &xformable) {
    bool reset;
    for (const UsdGeomXformOp &op : xformable.GetOrderedXformOps(&reset)) {
        auto type = op.GetOpType();
        switch (type) {
            case UsdGeomXformOp::TypeRotateXYZ:
                return EulerOrder::EULER_ORDER_XYZ;
            case UsdGeomXformOp::TypeRotateXZY:
                return EulerOrder::EULER_ORDER_XZY;
            case UsdGeomXformOp::TypeRotateYXZ:
                return EulerOrder::EULER_ORDER_YXZ;
            case UsdGeomXformOp::TypeRotateYZX:
                return EulerOrder::EULER_ORDER_YZX;
            case UsdGeomXformOp::TypeRotateZXY:
                return EulerOrder::EULER_ORDER_ZXY;
            case UsdGeomXformOp::TypeRotateZYX:
                return EulerOrder::EULER_ORDER_ZYX;
            default:
                return EulerOrder::EULER_ORDER_XYZ;
        }
    }
    return EulerOrder::EULER_ORDER_XYZ;
}


}