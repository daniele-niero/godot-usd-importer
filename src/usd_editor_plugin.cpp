#include "usd_editor_plugin.h"

namespace godot {

void USDEditorPlugin::_enter_tree() {
    add_import_plugin(memnew(USDImporter));
}

void USDEditorPlugin::_exit_tree() {
    //remove_import_plugin(memnew(USDImporter));
}

}