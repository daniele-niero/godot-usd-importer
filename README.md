# Godot USD Importer

A Godot GDExtension that imports USD (Universal Scene Description) scenes into Godot.

This repository contains a Godot extension (UsdImporter) and helper scripts to build OpenUSD and the Godot GDExtension binding. It's intended for developers who want to integrate USD assets into Godot projects or experiment with USD import pipelines.

## Contents

- `addons/` - compiled extension outputs for debug/release targets
- `OpenUSD/` - OpenUSD source tree (subdirectory used to build USD)
- `src/` - C++ source files for the GDExtension (USD importer implementation)
- `demo_project/` - a minimal Godot project demonstrating the importer
- `build.py` - high-level build script that orchestrates building USD and the GDExtension
- `SConstruct` - SCons build file used to compile the Godot extension

---

## Quick overview

The repository automates two main tasks:

1. Building OpenUSD (USD) into `usd/<variant>` using the provided OpenUSD sources and build scripts.
2. Building the Godot GDExtension (UsdImporter) using SCons and the Godot C++ bindings.

The `build.py` script is a convenience wrapper around these steps and copies required USD runtime files into the extension `bin` folder.

## Prerequisites

Ensure the following tools are installed and available on your PATH:

- Python 3.8+ (used by `build.py` and some USD build scripts)
- SCons (the project uses SCons to build the extension)
- A C/C++ compiler toolchain suitable for your platform (MSVC on Windows, Clang/GCC on macOS/Linux)
- CMake (required by OpenUSD build scripts)
- Ninja or another generator used by the OpenUSD build scripts (if required by your configuration)

On Windows specifically:

- You should use the Visual Studio Developer Command Prompt or ensure MSVC is available in your shell environment.
- Creating symlinks may require Administrator privileges or Developer Mode enabled. `build.py` will attempt to create a symlink into `demo_project/addons/UsdImporter` by default; pass `--no-symlink` if you prefer to copy files manually.

## Build instructions

The repository provides a high-level script `build.py` to perform the common build tasks. It accepts `--target` (debug or release) and other flags.

Example: build a release extension and install USD libraries

1. From the repository root, open a terminal and run (Windows PowerShell example):

```powershell
python .\build.py --target release --install-usd-libraries
```

2. To force-rebuild OpenUSD even if a build directory exists:

```powershell
python .\build.py --target release --force-build-usd
```

3. To build the debug variant:

```powershell
python .\build.py --target debug
```

4. To clean the built extension for a target:

```powershell
python .\build.py --target release --clean
```

What `build.py` does under the hood

- Runs the OpenUSD build script (`OpenUSD/build_scripts/build_usd.py`) to produce `usd/<variant>` when needed.
- Calls `scons` (configured by the `godot-cpp/SConstruct` and top-level `SConstruct`) to build the GDExtension. The SCons invocation will create `compile_commands.json` and the built shared library under `addons/<variant>/UsdImporter/bin/`.
- Copies required USD runtime files (DLLs / shared libraries and schema folders) into the extension `bin` folder when `--install-usd-libraries` is specified.
- Optionally creates a symlink into the demo project's `addons/` folder (default target: `demo_project/addons/UsdImporter`). Use `--no-symlink` to skip this.

### Manual SCons build (alternative)

If you prefer running SCons directly, use the `scons` command with the platform and target. Examples (from repo root):

```powershell
# Release (default)
scons platform=windows target=template_release compiledb=yes

# Debug (template_debug variant)
scons platform=windows target=template_debug debug_symbols=yes optimize=none compiledb=yes debug_crt=yes
```

The SCons build will place compiled objects under `build/<variant>/` and the shared library under `addons/<variant>/UsdImporter/bin/`.

## Notes and troubleshooting

- If you see issues related to missing USD libraries at runtime, rerun `build.py` with `--install-usd-libraries` to copy them into the extension `bin` folder.
- If `compile_commands.json` is produced, `build.py` will attempt to patch it to add extra compiler warning flags for unused variables/parameters. This is a convenience for static analysis and can be disabled by editing `build.py`.
- On Windows, ensure the same C runtime (debug/release) is used for both the extension and USD libraries. The SCons `SConstruct` file adjusts MSVC flags for debug builds (appends `/MDd`).

## Demo project

Open `demo_project/project.godot` in Godot 4.x (matching your compiled GDExtension target). If the `demo_project/addons/UsdImporter` symlink was created by `build.py`, the extension will be available in the project. Otherwise, copy the built `addons/<variant>/UsdImporter` folder into the demo project's `addons` directory manually.

## Contributing

Contributions are welcome. Please open issues or pull requests with changes. For build-related patches, describe the platform and toolchain used to test your changes.

## License

See repository `LICENSE` and `THIRD_PARTY_NOTICES` files
