#include "usd_visitor.h"


PXR_NAMESPACE_USING_DIRECTIVE
using namespace godot;
namespace godot_usd_importer {


UsdVisitorRegistry& UsdVisitorRegistry::get_instance() {
    static UsdVisitorRegistry instance;
    return instance;
}

void UsdVisitorRegistry::register_delegate(const TfToken& typeName, PrimImportDelegate delegate){
    delegates[typeName] = delegate;
}

const PrimImportDelegateMap& UsdVisitorRegistry::get_delegates() const {
    return delegates;
}

PrimImportDelegate UsdVisitorRegistry::find_delegate_for_prim(const UsdPrim &prim) {
    PrimImportDelegate best_delegate = nullptr;

    TfType best_usd_type = TfType::GetUnknownType();
    for (const auto& [type_name, delegate] : delegates) {
        TfType usd_type = TfType::FindByName(type_name);
        if (usd_type && prim.IsA(usd_type)) {
            if (!best_delegate || usd_type.IsA(best_usd_type)) {
                best_delegate = delegate;
                best_usd_type = usd_type;
            }
        }
    }

    return best_delegate;
}


void UsdVisitor::visit(const UsdPrim& prim, Node3D* parent_node, Node3D* scene_root) {

    Node3D* new_node = nullptr;
    auto delegate = UsdVisitorRegistry::get_instance().find_delegate_for_prim(prim);

    if (delegate)
        new_node = delegate(prim); // Call delegate

    if (new_node) {
        new_node->set_name(String(prim.GetName().GetText()));
        parent_node->add_child(new_node);
        new_node->set_owner(scene_root);
    } else {
        new_node = parent_node;
    }

    for (const auto& child : prim.GetChildren()) {
        visit(child, new_node, scene_root);
    }
}

Node3D* UsdVisitor::build_godot_scene(const UsdStageRefPtr stage, const String& scene_name) {
    if (!stage) {
        print_error("Invalid USD stage.");
        return nullptr;
    }

    Node3D* scene_root = memnew(Node3D);  // free it with scene_root->queue_free();
    scene_root->set_name(scene_name);

    UsdPrim root_prim = stage->GetPseudoRoot();
    visit(root_prim, scene_root, scene_root);

    return scene_root;
}

} // namespace godot
