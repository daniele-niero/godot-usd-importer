#pragma once

#include <godot_cpp/core/class_db.hpp>

namespace godot_usd_importer {

void initialize_usd_importer(godot::ModuleInitializationLevel p_level);
void uninitialize_usd_importer(godot::ModuleInitializationLevel p_level);

}
