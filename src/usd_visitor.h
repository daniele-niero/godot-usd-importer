#pragma once

#include <pxr/base/tf/hash.h>
#include <pxr/base/tf/token.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/stage.h>

#include <godot_cpp/classes/node3d.hpp>

#include <functional>
#include <unordered_map>


PXR_NAMESPACE_USING_DIRECTIVE
namespace godot {

using PrimImportDelegate = std::function<Node3D*(const UsdPrim&)>;
using PrimImportDelegateMap = std::unordered_map<TfToken, PrimImportDelegate, TfToken::HashFunctor>;

class UsdVisitorRegistry {
public:
    // Access the singleton registry
    static UsdVisitorRegistry& get_instance();

    void register_delegate(const TfToken& typeName, PrimImportDelegate delegate);
    const PrimImportDelegateMap& get_delegates() const;
    PrimImportDelegate find_delegate_for_prim(const UsdPrim &prim);

private:
    PrimImportDelegateMap delegates;

    // Private constructor / disable copy
    UsdVisitorRegistry() = default;
    UsdVisitorRegistry(const UsdVisitorRegistry&) = delete;
    UsdVisitorRegistry& operator=(const UsdVisitorRegistry&) = delete;
};


class UsdVisitor {
public:
    void visit(const UsdPrim& prim, Node3D* parent_node, Node3D* scene_root);
    Node3D* build_godot_scene(const UsdStageRefPtr stage, const String& scene_name);
};


} // namespace godot
