#include "usd_importer.h"
#include "godot_cpp/classes/global_constants.hpp"
#include "usd_visitor.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/classes/project_settings.hpp>

// USD headers
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usdGeom/mesh.h>

PXR_NAMESPACE_USING_DIRECTIVE
using namespace godot;
namespace godot_usd_importer {

USDImporter::USDImporter() = default;
USDImporter::~USDImporter() = default;

String USDImporter::_get_importer_name() const {
    return "usd.importer";
}

String USDImporter::_get_visible_name() const {
    return "USD Importer";
}

int32_t USDImporter::_get_preset_count() const {
    return 1; // Only one preset by default
}

String USDImporter::_get_preset_name(int32_t p_index) const {
    if (p_index == 0) {
        return "Default";
    }
    return "";
}

PackedStringArray USDImporter::_get_recognized_extensions() const {
    PackedStringArray exts;
    exts.append("usd");
    exts.append("usda");
    exts.append("usdc");
    return exts;
}

TypedArray<Dictionary> USDImporter::_get_import_options(const String &path, int32_t preset_index) const {
    TypedArray<Dictionary> options;

    Dictionary zup_option;
    zup_option["name"] = "zup_conversion";
    zup_option["default_value"] = true;
    options.append(zup_option);

    return options;
}

String USDImporter::_get_save_extension() const {
    return "tscn";
}

String USDImporter::_get_resource_type() const {
    return "PackedScene";
}

float USDImporter::_get_priority() const {
    return 1.0; // or another value; higher means higher priority
}

int32_t USDImporter::_get_import_order() const {
    return IMPORT_ORDER_DEFAULT; // or another order value
}

int32_t USDImporter::_get_format_version() const {
    return 1; // Version of the importer
}

bool USDImporter::_get_option_visibility(const String &path, const StringName &option_name, const Dictionary &options) const {
    // For now, we can return true for all options
    return true;
}

bool USDImporter::_can_import_threaded() const {
    return false; // USD import is not threaded for now
}

Error USDImporter::_import(
        const String &source_file, const String &save_path, const Dictionary &options,
        const TypedArray<String> &platform_variants, const TypedArray<String> &gen_files) const
{
    Ref<PackedScene> scene;
    scene.instantiate();

    // Convert Godot resource path to absolute filesystem path
    String godot_path = source_file;
    if (godot_path.begins_with("res://")) {
        godot_path = ProjectSettings::get_singleton()->globalize_path(godot_path);
    }
    print_line("Resolved USD file path: " + godot_path);

    // Convert Godot String to std::string safely
    CharString path_utf8 = godot_path.utf8();       // keep the CharString alive
    std::string std_path(path_utf8.get_data());     // copy into std::string

    // Attempt to open the USD stage
    UsdStageRefPtr stage = UsdStage::Open(std_path);
    if (!stage) {
        print_error("Failed to open USD stage: " + godot_path);
        return ERR_CANT_OPEN;
    }

    // name root node as the usd file name without extension
    String root_name = godot_path.get_file().get_basename();

    UsdVisitor visitor = UsdVisitor();
    Node3D* root_node = visitor.build_godot_scene(stage, root_name, options);

    scene->pack(root_node);
    String file_path = save_path + String(".") + _get_save_extension();
    print_line("Saving scene to: " + save_path);
    Error err = ResourceSaver::get_singleton()->save(scene, file_path);
    if (err != Error::OK) {
        print_error("Failed to save scene: " + file_path + " with error: " + itos(err));
        return err;
    }
    print_line("ALL GOOD!");
    return OK;
}

}