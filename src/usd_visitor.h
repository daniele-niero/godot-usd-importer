#pragma once

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/base/tf/token.h>

#include <functional>
#include <unordered_map>

#include <godot_cpp/classes/node3d.hpp>

PXR_NAMESPACE_USING_DIRECTIVE

namespace godot {

using PrimDelegate = std::function<Ref<Node3D>(UsdPrim&, Ref<Node3D>)>;
using PrimDelegateMap = std::unordered_map<TfToken, PrimDelegate>;

class UsdVisitorRegistry {
public:
    // Access the singleton registry
    static UsdVisitorRegistry& get_instance();

    void register_delegate(const TfToken& typeName, PrimDelegate delegate);
    const PrimDelegateMap& get_delegates() const;

private:
    PrimDelegateMap delegates;

    // Private constructor / disable copy
    UsdVisitorRegistry() = default;
    UsdVisitorRegistry(const UsdVisitorRegistry&) = delete;
    UsdVisitorRegistry& operator=(const UsdVisitorRegistry&) = delete;
};


class UsdVisitor {
public:
    void visit(UsdPrim& prim, const Ref<Node3D> parent_node);
    Ref<Node3D> UsdVisitor::build_godot_scene(const UsdStageRefPtr stage, const String &scene_name);
};


} // namespace godot
