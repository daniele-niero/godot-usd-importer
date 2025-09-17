#include "usd_visitor.h"

PXR_NAMESPACE_USING_DIRECTIVE

namespace godot {


UsdVisitorRegistry& UsdVisitorRegistry::get_instance() {
    static UsdVisitorRegistry instance;
    return instance;
}

void UsdVisitorRegistry::register_delegate(const TfToken& typeName, PrimDelegate delegate){
    delegates[typeName] = delegate;
}

const PrimDelegateMap& UsdVisitorRegistry::get_delegates() const {
    return delegates;
}


void UsdVisitor::visit(UsdPrim& prim, const Ref<Node3D> parent_node) {
    const PrimDelegateMap& delegates = UsdVisitorRegistry::get_instance().get_delegates();
    
    Ref<Node3D> new_parent;

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

Ref<Node3D> UsdVisitor::build_godot_scene(const UsdStageRefPtr stage, const String &scene_name) {
    if (!stage) {
        print_error("Invalid USD stage.");
        return nullptr;
    }

    Ref<Node3D> scene_root;
    scene_root.instantiate();
    scene_root->set_name(scene_name);

    UsdPrim root_prim = stage->GetPseudoRoot();
    for (const UsdPrim &prim : root_prim.GetChildren()) {
        visit(root_prim, scene_root);
    }

    return scene_root;
}

} // namespace godot
