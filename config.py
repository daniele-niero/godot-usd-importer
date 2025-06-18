# Configuration settings for the Godot USD Importer project

# Path to the Godot headers
GODOT_HEADERS_PATH = "path/to/godot/headers"

# Path to the Godot libraries
GODOT_LIBS_PATH = "path/to/godot/libs"

# Path to the third-party USD library
USD_LIB_PATH = "thirdparty/usd"

# Compiler flags
COMPILER_FLAGS = [
    "-std=c++17",
    "-Wall",
    "-Wextra",
]

# Additional include paths
INCLUDE_PATHS = [
    GODOT_HEADERS_PATH,
    USD_LIB_PATH,
]

# Output directory for the compiled extension
OUTPUT_DIR = "bin"

# Build options
BUILD_OPTIONS = {
    "debug": True,
    "optimize": False,
}