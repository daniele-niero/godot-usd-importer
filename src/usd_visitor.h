#pragma once

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/base/tf/token.h>
#include <pxr/base/tf/hash.h>

#include <functional>
#include <unordered_map>

#include <godot_cpp/classes/node3d.hpp>

PXR_NAMESPACE_USING_DIRECTIVE

// // Make TfToken hashable for std::unordered_map
// namespace std {
//     template <>
//     struct hash<pxr::TfToken> {
//         size_t operator()(const pxr::TfToken &t) const noexcept {
//             return std::hash<std::string>()(t.GetString());
//         }
//     };
// }


namespace godot {

using PrimImportDelegate = std::function<Node3D*(const UsdPrim&, Node3D*)>;
using PrimImportDelegateMap = std::unordered_map<TfToken, PrimImportDelegate>;

class UsdVisitorRegistry {
public:
    // Access the singleton registry
    static UsdVisitorRegistry& get_instance();

    void register_delegate(const TfToken& typeName, PrimImportDelegate delegate);
    const PrimImportDelegateMap& get_delegates() const;

private:
    PrimImportDelegateMap delegates;

    // Private constructor / disable copy
    UsdVisitorRegistry() = default;
    UsdVisitorRegistry(const UsdVisitorRegistry&) = delete;
    UsdVisitorRegistry& operator=(const UsdVisitorRegistry&) = delete;
};


class UsdVisitor {
public:
    void visit(const UsdPrim& prim, Node3D* parent_node);
    Node3D* build_godot_scene(const UsdStageRefPtr stage, const String& scene_name);
};


} // namespace godot
