#pragma once

#include <godot_cpp/classes/editor_plugin.hpp>

namespace godot_usd_importer {

class USDEditorPlugin : public godot::EditorPlugin {
    GDCLASS(USDEditorPlugin, EditorPlugin);

protected:
    static void _bind_methods() {}

public:
    godot::String _get_plugin_name() const override {
        return "USD Importer";
    }
    godot::String get_plugin_version() const {
        return "1.0";
    }
    void _enter_tree() override;
    void _exit_tree() override;
};

}