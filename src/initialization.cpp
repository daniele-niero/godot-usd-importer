#include "initialization.h"
#include "godot_cpp/core/print_string.hpp"
#include "usd_visitor.h"
#include "basic_import_delegates.h"
#include "mesh_import_delegates.h"

#include "usd_importer.h"
#include "usd_editor_plugin.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>


PXR_NAMESPACE_USING_DIRECTIVE
using namespace godot;

// #ifdef _DEBUG
// 	TfDebug::SetDebugSymbols(true);
//     TfDebug::SetTraceFile("usd_trace.log");
// #endif

static void register_import_delegates() {
	UsdVisitorRegistry& registry = UsdVisitorRegistry::get_instance();

	registry.register_delegate(TfToken("UsdGeomXformable"), import_xformable);
	registry.register_delegate(TfToken("UsdGeomMesh"), import_mesh);
	register_basic_solids();
}


using namespace godot;


void initialize_usd_importer(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		register_import_delegates();
		GDREGISTER_CLASS(USDImporter);
		GDREGISTER_CLASS(USDEditorPlugin);

		EditorPlugins::add_by_type<USDEditorPlugin>();
		print_line_rich("[color=grey]--- USD Importer Initialized ---[/color]");
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