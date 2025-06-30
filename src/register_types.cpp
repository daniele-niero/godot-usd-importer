#include "register_types.h"

#include "usd_importer.h"
#include "usd_editor_plugin.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void Initialize(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {	
		print_line("Initializing USD Importer...");
		GDREGISTER_CLASS(USDImporter);
		GDREGISTER_CLASS(USDEditorPlugin);
		EditorPlugins::add_by_type<USDEditorPlugin>();
	}
}

void Uninitialize(ModuleInitializationLevel p_level) {
	// if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
	// 	return;
	// }
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT ExtensionInit(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(Initialize);
	init_obj.register_terminator(Uninitialize);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_EDITOR);

	return init_obj.init();
}
}