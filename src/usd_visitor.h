#pragma once

#include <pxr/usd/usd/stage.h>
#include <godot_cpp/classes/node3d.hpp>

PXR_NAMESPACE_USING_DIRECTIVE

namespace godot {

class UsdToGodotVisitor {
public:
    UsdToGodotVisitor();
    ~UsdToGodotVisitor();

    Node3D* build_godot_scene(const UsdStageRefPtr stage, const String &scene_name);

private:
    Node3D* scene_root = nullptr;
    // Entry point: visit a USD prim and build Godot nodes under parent
    void visit(const pxr::UsdPrim &prim, Node *parent = nullptr);
};

} // namespace godot
