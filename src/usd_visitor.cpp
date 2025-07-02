#include "usd_visitor.h"
#include <pxr/usd/usdGeom/scope.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/base/gf/vec3f.h>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/surface_tool.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

PXR_NAMESPACE_USING_DIRECTIVE

namespace godot {

UsdToGodotVisitor::UsdToGodotVisitor() {}
UsdToGodotVisitor::~UsdToGodotVisitor() {}

// Helper: Convert USD Mesh to Godot ArrayMesh
static Ref<ArrayMesh> _usd_mesh_to_godot(const UsdGeomMesh &usd_mesh) {
    Ref<ArrayMesh> mesh;
    mesh.instantiate();
    Ref<SurfaceTool> st;
    st.instantiate();
    st->begin(Mesh::PRIMITIVE_TRIANGLES);

    VtArray<GfVec3f> points;
    usd_mesh.GetPointsAttr().Get(&points);
    VtArray<int> faceVertexCounts, faceVertexIndices;
    usd_mesh.GetFaceVertexCountsAttr().Get(&faceVertexCounts);
    usd_mesh.GetFaceVertexIndicesAttr().Get(&faceVertexIndices);

    // Normals (optional)
    VtArray<GfVec3f> normals;
    bool has_normals = usd_mesh.GetNormalsAttr().Get(&normals);

    // Orientation: rightHanded (default, CCW), leftHanded (CW)
    TfToken orientation_token;
    usd_mesh.GetOrientationAttr().Get(&orientation_token);
    bool reverse_winding = (orientation_token == TfToken("leftHanded"));

    int idx = 0;
    for (size_t f = 0; f < faceVertexCounts.size(); ++f) {
        int n = faceVertexCounts[f];
        // Collect polygon indices
        std::vector<int> poly_indices;
        for (int v = 0; v < n; ++v) {
            poly_indices.push_back(faceVertexIndices[idx + v]);
        }
        if (reverse_winding) {
            std::reverse(poly_indices.begin(), poly_indices.end());
        }
        // Triangulate faces with n >= 3
        for (int t = 0; t < n - 2; ++t) {
            int pi2 = poly_indices[0];
            int pi1 = poly_indices[t + 1];
            int pi0 = poly_indices[t + 2];
            Vector3 pos0(points[pi0][0], points[pi0][1], points[pi0][2]);
            Vector3 pos1(points[pi1][0], points[pi1][1], points[pi1][2]);
            Vector3 pos2(points[pi2][0], points[pi2][1], points[pi2][2]);
            if (has_normals) {
                if (pi0 < (int)normals.size()) st->set_normal(Vector3(normals[pi0][0], normals[pi0][1], normals[pi0][2]));
            }
            st->add_vertex(pos0);
            if (has_normals) {
                if (pi1 < (int)normals.size()) st->set_normal(Vector3(normals[pi1][0], normals[pi1][1], normals[pi1][2]));
            }
            st->add_vertex(pos1);
            if (has_normals) {
                if (pi2 < (int)normals.size()) st->set_normal(Vector3(normals[pi2][0], normals[pi2][1], normals[pi2][2]));
            }
            st->add_vertex(pos2);
        }
        idx += n;
    }
    st->generate_normals();
    mesh = st->commit();
    return mesh;
}

void UsdToGodotVisitor::visit(const UsdPrim &prim, Node *parent) {
    Node *node = nullptr;

    if (prim.IsA<UsdGeomScope>() || prim.IsA<UsdGeomXform>()) {
        node = memnew(Node3D);
        node->set_name(String(prim.GetName().GetText()));
        // TODO: Apply transform
    }

    else if (prim.IsA<UsdGeomMesh>()) {
        UsdGeomMesh usd_mesh(prim);
        Ref<ArrayMesh> mesh = _usd_mesh_to_godot(usd_mesh);
        MeshInstance3D *mi = memnew(MeshInstance3D);
        mi->set_name(String(prim.GetName().GetText()));
        mi->set_mesh(mesh);
        node = mi;
    }

    if (node) {
        if (parent) {
            parent->add_child(node);
        }
        print_line(scene_root);
        node->set_owner(scene_root);
        for (const UsdPrim &child : prim.GetChildren())
            visit(child, node);
    } else{
        for (const UsdPrim &child : prim.GetChildren())
            visit(child, parent);
    }
}

Node3D* UsdToGodotVisitor::build_godot_scene(const UsdStageRefPtr stage, const String &scene_name) {
    if (!stage) {
        print_error("Invalid USD stage.");
        return nullptr;
    }

    scene_root = memnew(Node3D);
    scene_root->set_name(scene_name);

    UsdPrim root_prim = stage->GetPseudoRoot();
    for (const UsdPrim &prim : root_prim.GetChildren()) {
        visit(prim, scene_root);
    }

    return scene_root;
}

} // namespace godot
