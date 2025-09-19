#pragma once

#include <godot_cpp/classes/editor_plugin.hpp>

namespace godot {

class USDEditorPlugin : public EditorPlugin {
    GDCLASS(USDEditorPlugin, EditorPlugin);

protected:
    static void _bind_methods() {}

public:
    String _get_plugin_name() const override {
        return "USD Importer";
    }
    String get_plugin_version() const {
        return "1.0";
    }
    void _enter_tree() override;
    void _exit_tree() override;
};

}