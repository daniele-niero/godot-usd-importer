#!/usr/bin/env python
import os
import sys
import subprocess
from functools import partial
from scons_hints import *


vars = Variables()
vars.Add('force_usd_build', 'Force USD rebuild (default: 0)', 0)  # Default is 0 (off)
try:
    Import('env')
    print("SUCESSFULLY IMPORT ENV")
except:
    print("MAKE CUSTOM ENV")
    env = Environment(variables=vars, ENV=os.environ)
vars.Update(env)
print(vars.UnknownVariables())
Help(vars.GenerateHelpText(env))

if env.get('target') == 'template_debug':
    usd_build_dir = "usd_build/debug"
else:
    usd_build_dir = "usd_build/release"
usd_include_path = os.path.join(usd_build_dir, "include")
usd_lib_path = os.path.join(usd_build_dir, "lib")
usd_bin_path = os.path.join(usd_build_dir, "bin")


def build_usd(target, source, env):
    build_variant = "release"
    if env.get('target') == "template_debug":
        build_variant = "debug"

    print(build_variant)
            
    usd_build_script = source[0].abspath
    cmd = [
        sys.executable, usd_build_script,
        "--no-python", "--no-examples", "--no-tutorials", "--no-tools",
        "--no-materialx", "--no-imaging", "--build-monolithic", 
        "--build-variant", build_variant,
        f'../{usd_build_dir}'
    ]
    subprocess.check_call(cmd, cwd="OpenUSD")

    # Touch the correct marker file after build
    with open(target[0].get_abspath(), "w") as f:
        f.write(f"USD {build_variant} build marker\n")


env.Append(LIBPATH=[usd_lib_path])

usd_marker = os.path.join(usd_build_dir, ".usd_built")
usd_build = env.Command(
    target=usd_marker,
    source="OpenUSD/build_scripts/build_usd.py",
    action=build_usd
)
# If user requested force build, always rebuild
if int(env.get('force_usd_build', 0)):
    AlwaysBuild(usd_build)

Return("env", "usd_build", "usd_build_dir", "usd_include_path", "usd_lib_path", "usd_bin_path")