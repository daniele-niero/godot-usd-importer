#pragma once

#include <godot_cpp/classes/editor_plugin.hpp>

namespace godot {

class USDEditorPlugin : public EditorPlugin {
    GDCLASS(USDEditorPlugin, EditorPlugin);

protected:
    static void _bind_methods() {}

public:
    void _enter_tree() override;
    void _exit_tree() override;
};

}