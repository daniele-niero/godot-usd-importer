#include "initialization.h"
#include "usd_visitor.h"
#include "mesh_import_delegates.h"

#include "usd_importer.h"
#include "usd_editor_plugin.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <pxr/base/tf/token.h>

PXR_NAMESPACE_USING_DIRECTIVE
using namespace godot;

static void register_import_delegates() {
	UsdVisitorRegistry& registry = UsdVisitorRegistry::get_instance();

	registry.register_delegate(TfToken("Cube"), import_cube);
}


void initialize_usd_importer(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		print_line("Initializing USD Importer...");
		register_import_delegates();
		GDREGISTER_CLASS(USDImporter);
		GDREGISTER_CLASS(USDEditorPlugin);
		EditorPlugins::add_by_type<USDEditorPlugin>();
	}
}

void uninitialize_usd_importer(ModuleInitializationLevel p_level) {
	// if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
	// 	return;
	// }
}

extern "C" {
	// Entry Point.
	GDExtensionBool GDE_EXPORT ExtensionInit(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_usd_importer);
		init_obj.register_terminator(uninitialize_usd_importer);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_EDITOR);

		return init_obj.init();
	}
}