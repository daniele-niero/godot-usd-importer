#include "basic_import_delegates.h"
#include "pxr/base/gf/vec3d.h"
#include "pxr/base/gf/transform.h"
#include "pxr/usd/usd/common.h"
#include "utils.h"

#include <pxr/usd/usdGeom/metrics.h>
#include <pxr/usd/usdGeom/xformable.h>
#include <pxr/usd/usdGeom/xformCommonAPI.h>
#include <pxr/base/gf/matrix4d.h>

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/transform3d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>


PXR_NAMESPACE_USING_DIRECTIVE
using namespace godot;
namespace godot_usd_importer {


void set_node_transform(Node3D* gd_node, const UsdPrim &usd_prim, const bool convert_if_zup) {
    if (!usd_prim) return;
    UsdStageWeakPtr stage = usd_prim.GetStage();
    TfToken up_axis = UsdGeomGetStageUpAxis(stage);
    double meters_per_unit = UsdGeomGetStageMetersPerUnit(stage);

    UsdGeomXformable xformable(usd_prim);
    if (!xformable) return;

    UsdGeomXformCommonAPI xformable_api(usd_prim);

    GfMatrix4d usd_mat;
    bool resets;
    xformable.GetLocalTransformation(&usd_mat, &resets, UsdTimeCode::Default());
    if (up_axis == UsdGeomTokens->z && convert_if_zup == true) {
        usd_mat = ZupToYup(usd_mat);
    }

    gd_node->set_rotation_order(get_rotation_order(xformable));
    usd_mat = usd_mat.RemoveScaleShear();
    Basis gd_basis = Basis(
        Vector3(usd_mat[0][0], usd_mat[0][1], usd_mat[0][2]),
        Vector3(usd_mat[1][0], usd_mat[1][1], usd_mat[1][2]),
        Vector3(usd_mat[2][0], usd_mat[2][1], usd_mat[2][2])
    );
    // scale
    GfVec3d scl = GfTransform(usd_mat).GetScale();
    gd_basis[0] = gd_basis[0] * scl[0];
    gd_basis[1] = gd_basis[1] * scl[1];
    gd_basis[2] = gd_basis[2] * scl[2];

    Transform3D gd_transform;
    gd_transform.basis = gd_basis;

    gd_transform.origin = Vector3(usd_mat[3][0], usd_mat[3][1], usd_mat[3][2]) * meters_per_unit;

    gd_node->set_transform(gd_transform);
}

Node3D* import_xformable(const UsdPrim &usd_prim, const Dictionary &options) {
    Node3D* node = memnew(Node3D);
    bool convert_if_zup = options.get("zup_conversion", true);
    set_node_transform(node, usd_prim, convert_if_zup);
    return node;
}


} // namespace godot