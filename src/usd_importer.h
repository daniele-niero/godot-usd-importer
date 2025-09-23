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


namespace godot_usd_importer {

class USDImporter : public godot::EditorImportPlugin {
    GDCLASS(USDImporter, EditorImportPlugin);

protected:
    static void _bind_methods() {}

public:
    USDImporter();
    ~USDImporter();

    godot::String                           _get_importer_name()         const override;
    godot::String                           _get_visible_name()          const override;
    int32_t                                 _get_preset_count()          const override;
    godot::String                           _get_preset_name(int32_t p_index) const override;
    godot::PackedStringArray                _get_recognized_extensions() const override;
    godot::TypedArray<godot::Dictionary>    _get_import_options(const godot::String &p_path, int32_t p_preset_index) const override;
    godot::String                           _get_save_extension()        const override;
    godot::String                           _get_resource_type()         const override;
    float                                   _get_priority()              const override;
    int32_t                                 _get_import_order()          const override;
    int32_t                                 _get_format_version()        const override;
    bool                                    _get_option_visibility(const godot::String &p_path, const godot::StringName &option_name, const godot::Dictionary &options) const override;
    bool                                    _can_import_threaded()       const override;
    godot::Error                            _import(const godot::String &source_file, const godot::String &save_path, const godot::Dictionary &options,
                                                    const godot::TypedArray<godot::String> &platform_variants, const godot::TypedArray<godot::String> &gen_files) const override;
};

}