import os
from scons_hints import *


usd_importer_bin = "UsdImporter/bin"

vars = Variables()
vars.Add('force_usd_build', 'Force USD rebuild (default: 0)', 0)  # Default is 0 (off)
env = Environment(variables=vars, ENV=os.environ)
vars.Update(env)
Help(vars.GenerateHelpText(env))
Export('env')

Import("env")

env = SConscript("godot-cpp/SConstruct", export={'env': env})
Export('env')

# Load USD logic
env, usd_build, usd_build_dir, usd_include_path, usd_lib_path, usd_bin_path = SConscript("UsdSConscript.py")

VariantDir('build', 'src', duplicate=0)


env.Append(CPPPATH=["build/", usd_include_path])
env.Append(LIBPATH=[usd_lib_path])
sources = Glob("build/*.cpp")

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

# Copy dependencies
runtime_deps = [
    os.path.join(usd_bin_path, "tbb.dll"),
    os.path.join(usd_lib_path, "usd_ms.dll"),
]

dll_installs = env.Install(usd_importer_bin, runtime_deps)
schemas_install = env.Install(usd_importer_bin, os.path.join(usd_lib_path, "usd"))

# Make runtime deps depend on USD build
env.Depends(dll_installs, usd_build)
env.Depends(schemas_install, usd_build)
env.Depends(library, usd_build)

Default([library, dll_installs, schemas_install])
