#!/usr/bin/env python
import os
import sys

# python .\build_scripts\build_usd.py --no-python --no-examples --no-tutorials --no-tools --no-materialx --no-imaging --build-monolithic --dry_run ../usd

# Use 'build' as the build output directory, do not duplicate source files
VariantDir('build', 'src', duplicate=0)

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["build/"])
sources = Glob("build/*.cpp")

usd_include_path = "usd/include"  # Change to your USD include path
usd_lib_path = "usd/lib"          # Change to your USD lib path

env.Append(CPPPATH=[usd_include_path])
env.Append(LIBPATH=[usd_lib_path])


# Link against all required static USD and TBB libraries (add more as needed, and ensure they exist)
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
    # add any other tbb-related libraries you see in your C:/USD/lib directory, without the .lib extension
])

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "GodotUsdImporter/bin/usd_importer.{}.{}.framework/libgdexample.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "GodotUsdImporter/bin/usd_importer.{}.{}.simulator.a".format(env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "GodotUsdImporter/bin/usd_importer.{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "GodotUsdImporter/bin/usd_importer{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
