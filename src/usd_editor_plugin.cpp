#include "usd_editor_plugin.h"
#include "usd_importer.h"

using namespace godot;
namespace godot_usd_importer {


void USDEditorPlugin::_enter_tree() {
    add_import_plugin(memnew(USDImporter));
}

void USDEditorPlugin::_exit_tree() {

}


}