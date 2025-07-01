# THIS IS A WORK IN PROGRESS! #


# Godot USD Importer

## Overview
The Godot USD Importer is a C++ extension for the Godot Engine that allows users to import Pixar's Universal Scene Description (USD) files into their Godot projects. This extension provides functionality to load, parse, and convert USD files into a format that Godot can understand, facilitating the integration of complex 3D assets created with USD.

## Features
- Load and parse USD files.
- Convert USD data into Godot-compatible formats.
- Support for various USD features and data structures.
- Easy integration with the Godot editor.

## Project Structure
```
godot-usd-importer
├── src
│   ├── usd_importer.cpp       # Implementation of the USD importer functionality.
│   ├── usd_importer.h         # Header file declaring classes and functions for the importer.
│   └── register_types.cpp      # Registers the USD importer with the Godot engine.
├── include
│   └── usd_importer.h         # Public interface for the USD importer.
├── thirdparty
│   └── usd                    # Third-party USD library files.
├── SConstruct                 # Build script for compiling the extension.
├── config.py                  # Configuration settings for the build process.
├── README.md                  # Documentation for the project.
└── LICENSE                    # Licensing information for the project.
```

## Installation
1. Clone the repository to your local machine.
2. Ensure you have the necessary dependencies installed, including the Godot Engine and the USD library.
3. Navigate to the project directory and run the build script using SCons:
   ```
   scons platform=windows
   ```
4. Once built, the extension can be added to your Godot project by copying the compiled files to the appropriate directory.

## Usage
To use the USD importer in your Godot project:
1. Import your USD files using the provided importer functionality.
2. Access the converted assets within the Godot editor.
3. Utilize the imported assets in your game scenes as needed.

## Contributing
Contributions to the Godot USD Importer are welcome! Please submit a pull request or open an issue for any enhancements or bug fixes.

## License
This project is licensed under the terms specified in the LICENSE file. Please review the license for details on usage and distribution.
