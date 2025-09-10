#include "usd_visitor.h"
#include "mesh_import_utils.h"
#include <pxr/usd/usdGeom/scope.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/base/gf/vec3f.h>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/surface_tool.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/mesh.hpp>

PXR_NAMESPACE_USING_DIRECTIVE

namespace godot {

UsdToGodotVisitor::UsdToGodotVisitor() {}
UsdToGodotVisitor::~UsdToGodotVisitor() {}

void UsdToGodotVisitor::visit(const UsdPrim &prim, Node *parent) {
    Node *node = nullptr;

    UtilityFunctions::print("Visiting USD Prim: ", prim.GetName().GetText());
    UtilityFunctions::print("   is it geom: ", prim.IsA<UsdGeomGprim>());

    if (prim.IsA<UsdGeomScope>() || prim.IsA<UsdGeomXform>()) {
        node = memnew(Node3D);
        node->set_name(String(prim.GetName().GetText()));
        // TODO: Apply transform
    }
    else if (prim.IsA<UsdGeomGprim>()) {
        Ref<Mesh> mesh = UsdMeshImportUtils::import_mesh_from_prim(prim);
        MeshInstance3D *mi = memnew(MeshInstance3D);
        mi->set_name(String(prim.GetName().GetText()));
        mi->set_mesh(mesh);
        node = mi;
    }

    if (node) {
        if (parent) {
            parent->add_child(node);
        }
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
