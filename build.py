#!/usr/bin/env python3
import argparse
import subprocess
import sys
import platform
import shutil
from pathlib import Path
from terminal_ansi import Terminal


def run(cmd, cwd=None):
    result = subprocess.run(cmd, cwd=cwd)
    if result.returncode != 0:
        Terminal.Red(f"❗Command failed with exit code {result.returncode}: {' '.join(cmd)}")
        sys.exit(result.returncode)


def build_usd(args: argparse.Namespace, extension_dir: Path):
    variant = "release"
    if args.target == "debug":
        variant = "debug"

    open_usd_root = Path("OpenUSD")
    usd_build_dir = Path(f"usd/{variant}")

    if args.clean:
        Terminal.Blue(f"Clean Usd {variant} from {usd_build_dir}")
        shutil.rmtree(str(usd_build_dir))
        return

    if args.force_build_usd:
        shutil.rmtree(str(usd_build_dir))

    build_usd_script = open_usd_root.joinpath("build_scripts", "build_usd.py")
    need_installing = args.install_usd_libraries

    if usd_build_dir.exists() == False:
        need_installing = True
        Terminal.Blue(f'⚙️  Building USD {variant} in ".\{usd_build_dir}" (this will take some time) ...')
        usd_cmd = [
            sys.executable, str(build_usd_script),
            "--no-python", "--no-examples", "--no-tutorials", "--no-tools", "--no-materialx", "--no-imaging",
            "--quiet",
            "--build-monolithic",
            f"--build-variant", variant,
            str(usd_build_dir)
        ]
        run(usd_cmd)
        Terminal.Green("\n✅ USD Build finished successfully.\n")

    if need_installing:
        Terminal.Blue(f'📦 Install USD in Extension Directory ...')

        usd_bin_path = usd_build_dir.joinpath("bin")
        usd_lib_path = usd_build_dir.joinpath("lib")

        extension_bin_dir = extension_dir.joinpath('bin')

        # copy USD folder (schemas and such)
        Terminal.Default('Copying "usd" folder (schemas and such) ...')
        schemas_dest_dir = extension_bin_dir.joinpath('usd')
        if schemas_dest_dir.exists():
            shutil.rmtree(str(schemas_dest_dir))
        schemas_source_dir = usd_lib_path.joinpath('usd')
        shutil.copytree(str(schemas_source_dir), str(schemas_dest_dir))
        Terminal.Inline('Copying "usd" folder (schemas and such) Done!')

        # copy all shared libraries from usd_bin_path (most likely only one dll, since we built Usd as "monolithic")
        for dll_file in usd_lib_path.glob('*.dll'):
            Terminal.Inline(f'Copying {dll_file} folder (schemas and such) ...')
            shutil.copyfile(str(dll_file), str(extension_bin_dir.joinpath(dll_file.name)))
            Terminal.Inline(f'Copying {dll_file} folder (schemas and such) Done!')

        # tbb is a dependency of Usd's dll
        if variant == 'debug':
            for dll_file in usd_bin_path.glob('*.dll'):
                Terminal.Inline(f'Copying {dll_file} folder (schemas and such) ...')
                shutil.copyfile(str(dll_file), str(extension_bin_dir.joinpath(dll_file.name)))
                Terminal.Inline(f'Copying {dll_file} folder (schemas and such) Done!')
        else:
            tbb_dll = usd_bin_path.joinpath('tbb.dll')
            Terminal.Inline(f'Copying {tbb_dll} folder (schemas and such) ...')
            shutil.copyfile(str(tbb_dll), str(extension_bin_dir.joinpath(tbb_dll.name)))
            Terminal.Inline(f'Copying {tbb_dll} folder (schemas and such) Done!')
            
        Terminal.Green("\n✅ Installed USD in Extesnion Directory successfully.\n")


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
            'use_static_cpp=no',
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
        Terminal.Blue(f'Clean UsdImporter')

        scons_cmd.append('--clean')
        run(scons_cmd)
    else:
        Terminal.Blue(f'⚙️  Buiding UsdImporter ...')

        run(scons_cmd)

        Terminal.Green("\n✅ UsdImporter Built Successfully.\n")


def create_symlink(target: Path, link_path: Path):
    try:
        # Remove old symlink or directory if it exists
        if link_path.is_symlink() or link_path.exists():
            if link_path.is_dir() and not link_path.is_symlink():
                shutil.rmtree(link_path)
            else:
                link_path.unlink()

        # Ensure parent directories exist
        link_path.parent.mkdir(parents=True, exist_ok=True)

        link_path.symlink_to(target.resolve(), target_is_directory=target.is_dir())
        Terminal.Green(f"Symlink created: {link_path} -> {target}")

    except OSError as e:
        Terminal.Red(f"❗Error creating symlink in demo_project: {e}", file=sys.stderr)


def main(args: argparse.Namespace):
    extension_dir = Path('addons', args.target, 'UsdImporter')

    build_usd(args, extension_dir)
    build_gdextension(args, extension_dir)
    if args.no_symlink is False:
        create_symlink(extension_dir, Path('demo_project', 'addons', 'UsdImporter'))

    Terminal.Green("\n✅ BUILD FINISHED!.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Build USD + Godot GDExtension")

    parser.add_argument("--target", choices=["debug", "release"], default='release',
                        help="Build target (debug or release)")

    parser.add_argument("--force-build-usd", action="store_true", default=False,
                        help="Build USD even if it was build already")
    
    parser.add_argument("--install-usd-libraries", action="store_true", default=False,
                        help="Copy again USD's library along side the builded extension")

    parser.add_argument("--clean", action="store_true", default=False,
                        help="Clean the built extension for the specified target")
    
    parser.add_argument("--no-symlink", action="store_true", default=False,
                        help="Don't symlink the compiled extension into demo-project's addons folder")

    args = parser.parse_args()
    main(args)
