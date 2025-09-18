// src/usd_importer.h
#pragma once

#include <godot_cpp/classes/editor_import_plugin.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/surface_tool.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/box_mesh.hpp>


namespace godot {

class USDImporter : public EditorImportPlugin {
    GDCLASS(USDImporter, EditorImportPlugin);

protected:
    static void _bind_methods() {}

public:
    USDImporter();
    ~USDImporter();

    String                  _get_importer_name()         const override;
    String                  _get_visible_name()          const override;
    int32_t                 _get_preset_count()          const override;
    String                  _get_preset_name(int32_t p_index) const override;
    PackedStringArray       _get_recognized_extensions() const override;
    TypedArray<Dictionary>  _get_import_options(const String &p_path, int32_t p_preset_index) const override;
    String                  _get_save_extension()        const override;
    String                  _get_resource_type()         const override;
    float                   _get_priority()              const override;
    int32_t                 _get_import_order()          const override;
    int32_t                 _get_format_version()        const override;
    bool                    _get_option_visibility(const String &p_path, const StringName &option_name, const Dictionary &options) const override;
    bool                    _can_import_threaded()       const override;
    Error                   _import(const String &source_file, const String &save_path, const Dictionary &options,
                                    const TypedArray<String> &platform_variants, const TypedArray<String> &gen_files) const override;
};

}