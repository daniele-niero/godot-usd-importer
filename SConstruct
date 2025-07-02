#!/usr/bin/env python
import os
import sys
import shutil

# Paths
usd_build_dir = "usd"
usd_include_path = os.path.join(usd_build_dir, "include")
usd_lib_path = os.path.join(usd_build_dir, "lib")
usd_bin_path = os.path.join(usd_build_dir, "bin")
usd_importer_bin = "UsdImporter/bin"

# Step 1: Build USD if needed
usd_marker = os.path.join(usd_include_path, "pxr", "pxr.h")  # Marker file
usd_build_command = f"{sys.executable} OpenUSD/build_scripts/build_usd.py --no-python --no-examples --no-tutorials --no-tools --no-materialx --no-imaging --build-monolithic {usd_build_dir}"

usd_build = Command(
    target=usd_marker,
    source=[],
    action=usd_build_command,
    ENV=os.environ
)

# Step 2: Configure build
VariantDir('build', 'src', duplicate=0)
env = SConscript("godot-cpp/SConstruct")

# Make the extension depend on USD being built
env.Depends('.', usd_build)

env.Append(CPPPATH=["build/", usd_include_path])
env.Append(LIBPATH=[usd_lib_path])

sources = Glob("build/*.cpp")

# Link libraries
env.Append(LIBS=[
    "usd_ms",
    "tbb",
    "tbb_debug",
    "tbb_preview",
    "tbb_preview_debug",
    "tbbbind",
    "tbbbind_debug",
    "tbbmalloc",
    "tbbmalloc_debug",
    "tbbmalloc_proxy",
    "tbbmalloc_proxy_debug",
])

# Step 3: Build extension
if env["platform"] == "macos":
    library = env.SharedLibrary(
        f"UsdImporter/bin/usd_importer.{env['platform']}.{env['target']}.framework/libgdexample.{env['platform']}.{env['target']}",
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            f"UsdImporter/bin/usd_importer.{env['platform']}.{env['target']}.simulator.a",
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            f"UsdImporter/bin/usd_importer.{env['platform']}.{env['target']}.a",
            source=sources,
        )
else:
    library = env.SharedLibrary(
        f"UsdImporter/bin/usd_importer{env['suffix']}{env['SHLIBSUFFIX']}",
        source=sources,
    )

# Step 4: Copy runtime dependencies
runtime_deps = [
    os.path.join(usd_bin_path, "tbb.dll"),
    os.path.join(usd_lib_path, "usd_ms.dll"),
]

copy_commands = []

for dep in runtime_deps:
    target_path = os.path.join(usd_importer_bin, os.path.basename(dep))
    cmd = Command(target_path, dep, Copy("$TARGET", "$SOURCE"))
    Depends(cmd, usd_build)
    copy_commands.append(cmd)

# Copy folder: usd/lib/usd → UsdImporter/bin/usd
usd_runtime_folder_src = os.path.join(usd_lib_path, "usd")
usd_runtime_folder_dst = os.path.join(usd_importer_bin, "usd")

def copy_usd_folder(target, source, env):
    if os.path.exists(str(target[0])):
        shutil.rmtree(str(target[0]))
    shutil.copytree(str(source[0]), str(target[0]))
    return 0

usd_folder_cmd = Command(
    usd_runtime_folder_dst,
    usd_runtime_folder_src,
    copy_usd_folder
)
Depends(usd_folder_cmd, usd_build)
copy_commands.append(usd_folder_cmd)

# Step 5: Default build targets
Default([library] + copy_commands)
