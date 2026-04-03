#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/dictionary.hpp>

#include <pxr/base/tf/hash.h>
#include <pxr/base/tf/token.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/stage.h>

#include <functional>
#include <unordered_map>


namespace godot_usd_importer {

const pxr::TfToken EXCLUDE_FROM_IMPORT_ATTR = pxr::TfToken("godot_usd_importer:exclude");

using PrimImportDelegate = std::function<godot::Node3D*(const pxr::UsdPrim&, const godot::Dictionary &options)>;
using PrimImportDelegateMap = std::unordered_map<pxr::TfToken, PrimImportDelegate, pxr::TfToken::HashFunctor>;

class UsdVisitorRegistry {
public:
    // Access the singleton registry
    static UsdVisitorRegistry& get_instance();

    void register_delegate(const pxr::TfToken& typeName, PrimImportDelegate delegate);
    const PrimImportDelegateMap& get_delegates() const;
    PrimImportDelegate find_delegate_for_prim(const pxr::UsdPrim &prim);

private:
    PrimImportDelegateMap delegates;

    // Private constructor / disable copy
    UsdVisitorRegistry() = default;
    UsdVisitorRegistry(const UsdVisitorRegistry&) = delete;
    UsdVisitorRegistry& operator=(const UsdVisitorRegistry&) = delete;
};


class UsdVisitor {
public:
    void visit(const pxr::UsdPrim& prim, godot::Node3D* parent_node, godot::Node3D* scene_root, const godot::Dictionary &options);
    godot::Node3D* build_godot_scene(const pxr::UsdStageRefPtr stage, const godot::String& scene_name, const godot::Dictionary &options);
};


} // namespace godot_usd_importer
