#include "usd_visitor.h"

PXR_NAMESPACE_USING_DIRECTIVE

namespace godot {


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


void UsdVisitor::visit(const UsdPrim& prim, Node3D* parent_node) {
    const PrimImportDelegateMap& delegates = UsdVisitorRegistry::get_instance().get_delegates();

    Node3D* new_parent;

    auto it = delegates.find(prim.GetTypeName());
    if (it != delegates.end()) {
        new_parent = it->second(prim, parent_node); // Call delegate
    } else {
        new_parent = parent_node;
    }

    for (auto& child : prim.GetChildren()) {
        visit(child, new_parent);
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
    visit(root_prim, scene_root);

    return scene_root;
}

} // namespace godot
