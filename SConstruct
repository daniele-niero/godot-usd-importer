import os
from pathlib import Path
from scons_hints import *
from terminal_ansi import Terminal


env = SConscript("godot-cpp/SConstruct")

build_variant = "release"
if env.get('target') == "template_debug":
    build_variant = "debug"
    # Remove /MD if present
    env['CCFLAGS'] = [flag for flag in env['CCFLAGS'] if flag not in ['/MD', '/MT']]
    # Add /MDd
    env.Append(CCFLAGS=["/MDd"])

usd_build_dir = Path('usd', build_variant)
usd_include_path = usd_build_dir.joinpath("include")
usd_lib_path = usd_build_dir.joinpath("lib")

VariantDir(f'build/{build_variant}', 'src', duplicate=0)
env.Append(CPPPATH=[f'build/{build_variant}', str(usd_include_path)])
env.Append(LIBPATH=[str(usd_lib_path)])
sources = Glob(f'build/{build_variant}/*.cpp')

if env["platform"] == "macos":
    library = env.SharedLibrary(
        f"addons/{build_variant}/UsdImporter/bin/usd_importer.{env['platform']}.framework/libgdexample.{env['platform']}.{env['target']}",
        source=sources,
    )
else:
    library = env.SharedLibrary(
        f"addons/{build_variant}/UsdImporter/bin/usd_importer.{env['platform']}.{env['arch']}{env['SHLIBSUFFIX']}",
        source=sources,
    )


# usd_bin_path = usd_build_dir.joinpath("bin")
# usd_bin_path.glob
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
