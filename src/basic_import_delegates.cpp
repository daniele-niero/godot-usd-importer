#include "basic_import_delegates.h"
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


void set_node_transform(Node3D* gd_node, const UsdPrim &usd_prim) {
    if (!usd_prim) return;

    UsdGeomXformable xformable(usd_prim);
    if (!xformable) return;

    UsdGeomXformCommonAPI xformable_api(usd_prim);
    GfVec3d tr;
    GfVec3f rot, scl, piv;
    UsdGeomXformCommonAPI::RotationOrder rot_order; 
    xformable_api.GetXformVectors(&tr, &rot, &scl, &piv, &rot_order, UsdTimeCode::Default());

    // Usd only defines up axis y or z.
    // Godot is always y up, so the only case we need to handle is when usd is z up
    TfToken up_axis = UsdGeomGetStageUpAxis(usd_prim.GetStage());
    if (up_axis == UsdGeomTokens->z) {
        rot = GfVec3f(rot[0], -rot[2], rot[1]);
    }

    Basis gd_basis;
    gd_basis.set_euler(to_godot(rot), to_godot(rot_order));
    // scale
    gd_basis[0] = gd_basis[0] * scl[0];
    gd_basis[1] = gd_basis[1] * scl[1];
    gd_basis[2] = gd_basis[2] * scl[2];
    
    Transform3D gd_transform(gd_basis, Vector3(tr[0], tr[1], tr[2]));
    gd_node->set_transform(gd_transform);
}

Node3D* import_xformable(const UsdPrim &usd_prim) {
    Node3D* node = memnew(Node3D);
    set_node_transform(node, usd_prim);
    return node;
}


} // namespace godot