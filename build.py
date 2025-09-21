#!/usr/bin/env python3
import argparse
import subprocess
import sys
import json
import platform
import shutil
from pathlib import Path
from terminal_ansi import Terminal


def run(cmd, cwd=None):
    result = subprocess.run(cmd, cwd=cwd)
    if result.returncode != 0:
        Terminal.Red(f"❗Command failed with exit code {result.returncode}: {' '.join(cmd)}")
        sys.exit(result.returncode)


def patch_compile_commands(json_path: Path):
    """
    Patch compile_commands.json to append unused-variable-only warning flags
    to all C++ compilation commands.
    """
    Terminal.BoldYellow(f"Patching {json_path}")
    if not json_path.exists():
        Terminal.Red(f"{json_path} not found")
        return

    with json_path.open('r', encoding='utf-8') as f:
        data = json.load(f)

    patched = False
    for entry in data:
        cmd = entry.get("command")
        if not cmd:
            continue

        # Avoid adding flags multiple times
        if "-Wunused-variable" in cmd or "/W4" in cmd:
            continue

        # Platform-specific flags
        if "cl " in cmd or "cl.exe" in cmd:  # MSVC
            flags = "/W4"
        else:  # GCC/Clang
            flags = "-Wunused-variable -Wunused-parameter -Wunused-private-field"

        cmd = entry["command"]
        entry["command"] = f"{cmd} {flags}"
        patched = True

    if patched:
        with json_path.open('w', encoding='utf-8') as f:
            json.dump(data, f, indent=2)
        Terminal.Yellow(f"Patched {json_path} with unused-variable-only flags.")
    else:
        Terminal.Yellow(f"No changes made to {json_path}. Flags already present.")


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
        Terminal.Blue(f'⚙️  Building USD {variant} in "{usd_build_dir}" (this will take some time) ...')
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
        open(schemas_dest_dir.joinpath('.gdignore'), 'w').close()
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
        extra_cmd_args = [
            'use_static_cpp=no',
            'target=template_debug',
            'debug_symbols=yes',
            'optimize=none',
            'compiledb=yes',
        ]
        if system == 'windows':
            extra_cmd_args.append('debug_crt=yes')
    else:
        extra_cmd_args = ['target=template_release', 'compiledb=yes']

    scons_cmd = ["scons", f"platform={system}"] + extra_cmd_args

    if args.clean:
        Terminal.Blue(f'Clean UsdImporter')

        scons_cmd.append('--clean')
        run(scons_cmd)
    else:
        Terminal.Blue(f'⚙️  Buiding UsdImporter ...')
        run(scons_cmd)
        Terminal.Green("\n✅ UsdImporter Built Successfully.\n")
        patch_compile_commands(Path('compile_commands.json'))


def create_symlink(args: argparse.Namespace, extension_dir: Path):
    if args.no_symlink:
        return
    
    source = extension_dir
    target = Path(args.symlink_to)
    try:
        # Remove old symlink or directory if it exists
        if target.is_symlink() or target.exists():
            if target.is_dir() and not target.is_symlink():
                shutil.rmtree(target)
            else:
                target.unlink()

        # Ensure parent directories exist
        target.parent.mkdir(parents=True, exist_ok=True)

        target.symlink_to(source.resolve(), target_is_directory=True)
        Terminal.Green(f"Symlink created: {target} -> {source}")

    except OSError as e:
        Terminal.Red(f"❗Error creating symlink in demo_project: {e}", file=sys.stderr)
        if platform.system().lower() == 'windows':
            Terminal.Yellow("Even if Administrator, in Windows, to be able to create symlink, you might need to activate 'Developer Mode'")


def main(args: argparse.Namespace):
    extension_dir = Path('addons', args.target, 'UsdImporter')

    build_usd(args, extension_dir)
    build_gdextension(args, extension_dir)
    create_symlink(args, extension_dir)
   

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
    
    parser.add_argument("--symlink-to", default="demo_project/addons/UsdImporter", help="where to create a symbolic link to the compiled addon")

    args = parser.parse_args()
    main(args)
