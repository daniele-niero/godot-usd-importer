import os
from scons_hints import *


env = SConscript("godot-cpp/SConstruct")

build_variant = "release"
if env.get('target') == "template_debug":
    build_variant = "debug"

usd_importer_bin = f"UsdImporter/bin/{build_variant}"

usd_build_dir = f"usd/{build_variant}"
usd_include_path = os.path.join(usd_build_dir, "include")
usd_lib_path = os.path.join(usd_build_dir, "lib")
usd_bin_path = os.path.join(usd_build_dir, "bin")


VariantDir(f'build/{build_variant}', 'src', duplicate=0)
env.Append(CPPPATH=[f'build/{build_variant}', usd_include_path])
env.Append(LIBPATH=[usd_lib_path])
sources = Glob(f'build/{build_variant}/*.cpp')

if env["platform"] == "macos":
    library = env.SharedLibrary(
        f"UsdImporter/bin/{build_variant}/usd_importer.{env['platform']}.{env['target']}.framework/libgdexample.{env['platform']}.{env['target']}",
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            f"UsdImporter/bin/{build_variant}/usd_importer.{env['platform']}.{env['target']}.simulator.a",
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            f"UsdImporter/bin/{build_variant}/usd_importer.{env['platform']}.{env['target']}.a",
            source=sources,
        )
else:
    library = env.SharedLibrary(
        f"UsdImporter/bin/{build_variant}/usd_importer{env['suffix']}{env['SHLIBSUFFIX']}",
        source=sources,
    )


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

Default(library)
