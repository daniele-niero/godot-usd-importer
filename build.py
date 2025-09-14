#!/usr/bin/env python3
import argparse
import subprocess
import sys
import os
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
    usd_build = Path(f"usd/{variant}")

    build_usd_script = open_usd_root.joinpath("build_scripts", "build_usd.py")

    if usd_build.exists() == False or args.build_usd:
        print(f'---------- Build USD ----------')
        usd_cmd = [
            sys.executable, str(build_usd_script), #"--help"
            "--no-python", "--no-examples", "--no-tutorials", "--no-tools", "--no-materialx", "--no-imaging",
            # "--build-monolithic",
            "--inst", f"{usd_build}/INSTALLATION",
            f"--build-variant", variant,
            str(usd_build)
        ]
        run(usd_cmd)
        print("\n✅ USD Build finished successfully.\n")


def build_gdextension(args: argparse.Namespace, extension_dir: Path):
    if args.target == "debug":
        scons_target = "template_debug"
    else:
        scons_target = "template_release"

    print(f'---------- Build GDExtension ----------')

    scons_cmd = [
        "scons",
        f"platform=windows",
        f"target={scons_target}"
    ]
    run(scons_cmd, cwd=extension_dir)
    print("\n✅ GDExtension Build finished successfully.\n")


def main(args: argparse.Namespace):
    extension_dir = Path("UsdImporter")

    build_usd(args, extension_dir)
    # build_gdextension(args, extension_dir)

    print("\n✅ Build finished.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Build USD + Godot GDExtension")
    parser.add_argument("--target", choices=["debug", "release"], default='release',
                        help="Build target (debug or release)")
    parser.add_argument("--build-usd", action="store_true", default=False,
                        help="Build USD before building the extension")
    args = parser.parse_args()

    main(args)