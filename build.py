#!/usr/bin/env python3
import argparse
import subprocess
import sys
import platform
import shutil
from pathlib import Path

def run(cmd, cwd=None):
    print(f"\n>>> Running: {' '.join(cmd)}\n")
    result = subprocess.run(cmd, cwd=cwd)
    if result.returncode != 0:
        print(f"Command failed with exit code {result.returncode}: {' '.join(cmd)}")
        sys.exit(result.returncode)

def build_usd(args: argparse.Namespace, extension_dir: Path):
    variant = "release"
    if args.target == "debug":
        variant = "debug"
    
    open_usd_root = Path("OpenUSD")
    usd_build_dir = Path(f"usd/{variant}")

    build_usd_script = open_usd_root.joinpath("build_scripts", "build_usd.py")

    if usd_build_dir.exists() == False or args.force_build_usd:
        print(f'---------- Build USD ----------')
        usd_cmd = [
            sys.executable, str(build_usd_script),
            "--no-python", "--no-examples", "--no-tutorials", "--no-tools", "--no-materialx", "--no-imaging",
            "--build-monolithic",
            f"--build-variant", variant,
            str(usd_build_dir)
        ]
        run(usd_cmd)
        print("\n✅ USD Build finished successfully.\n")

        print(f'---------- Install USD in Extension Directory ----------')

        usd_bin_path = usd_build_dir.joinpath("bin")
        usd_lib_path = usd_build_dir.joinpath("lib")

        extension_bin_dir = extension_dir.joinpath('bin')

        # copy USD folder (schemas and such)
        schemas_dest_dir = extension_bin_dir.joinpath('usd')
        if schemas_dest_dir.exists():
            shutil.rmtree(str(schemas_dest_dir))
        schemas_source_dir = usd_lib_path.joinpath('usd')
        shutil.copytree(str(schemas_source_dir), str(schemas_dest_dir))
        
        # copy all shared libraries from usd_bin_path (most likely only one dll, since we built Usd as "monolithic")
        for dll_file in usd_lib_path.glob('*.dll'):
            shutil.copyfile(str(dll_file), str(extension_bin_dir.joinpath(dll_file.name)))

        # # tbb is a dependency of Usd's dll
        if variant == 'debug':
            tbb_dll = usd_bin_path.joinpath('tbb_debug.dll')
        else:
            tbb_dll = usd_bin_path.joinpath('tbb.dll')
        shutil.copyfile(str(tbb_dll), str(extension_bin_dir.joinpath(tbb_dll.name)))

        print("\n✅ Installed USD in Extesnion Directory successfully.\n")


def build_gdextension(args: argparse.Namespace, extension_dir: Path):
    supported_platforms = ['linux', 'macos', 'windows']
    system = platform.system().lower()
    if system not in supported_platforms:
        raise RuntimeError(f'Platform "{system}" is not supported - you cannot build on this platform')

    if args.target == "debug":
        scons_target = "template_debug"
        extra_cmd_args = [
            # 'use_hot_reload=yes', 
            # 'optimize=debug', 
            'debug_symbols=yes'
        ]
    else:
        scons_target = "template_release"
        extra_cmd_args = [
            # 'optimize=speed', 
            # 'debug_symbols=no'
        ]

    scons_cmd = ["scons", f"platform=windows", f"target={scons_target}"] + extra_cmd_args


    if args.clean:
        print(f'---------- Clean UsdImporter ----------')

        scons_cmd.append('--clean')
        run(scons_cmd)
        
        print("\n✅ UsdImporter Cleaned Successfully.\n")

    else:
        print(f'---------- Build UsdImporter ----------')

        run(scons_cmd)

        print("\n✅ UsdImporter Built Successfully.\n")


def main(args: argparse.Namespace):
    extension_dir = Path('addons', args.target, 'UsdImporter')

    build_usd(args, extension_dir)
    build_gdextension(args, extension_dir)

    print("\n✅ Build finished.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Build USD + Godot GDExtension")

    parser.add_argument("--target", choices=["debug", "release"], default='release',
                        help="Build target (debug or release)")

    parser.add_argument("--force-build-usd", action="store_true", default=False,
                        help="Build USD even if it was build already")
    
    parser.add_argument("--clean", action="store_true", default=False,
                        help="Clean the built extension for the specified target")

    args = parser.parse_args()
    main(args)
