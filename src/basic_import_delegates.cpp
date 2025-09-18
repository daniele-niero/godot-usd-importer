#include "basic_import_delegates.h"

#include <pxr/usd/usdGeom/xformable.h>
#include <pxr/base/gf/matrix4d.h>

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/transform3d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>


PXR_NAMESPACE_USING_DIRECTIVE
namespace godot {


void set_node_transform(Node3D* gd_node, const UsdPrim &usd_prim) {
    if (!usd_prim) return;

    UsdGeomXformable xformable(usd_prim);
    if (!xformable) return;

    // Compute local-to-world transform
    GfMatrix4d usd_mat;
    bool resets = false;
    xformable.GetLocalTransformation(&usd_mat, &resets, UsdTimeCode::Default());

    Transform3D gd_transform;
    gd_transform.basis = Basis(
        Vector3(usd_mat[0][0], usd_mat[0][1], usd_mat[0][2]),
        Vector3(usd_mat[1][0], usd_mat[1][1], usd_mat[1][2]),
        Vector3(usd_mat[2][0], usd_mat[2][1], usd_mat[2][2])
    );

    gd_transform.origin = Vector3(usd_mat[3][0], usd_mat[3][1], usd_mat[3][2]);

    gd_node->set_transform(gd_transform);
}

Node3D* import_xformable(const UsdPrim &usd_prim) {
    Node3D* node = memnew(Node3D);
    set_node_transform(node, usd_prim);
    return node;
}


} // namespace godot