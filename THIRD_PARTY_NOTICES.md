# Third-party notices and license summary

This repository includes or depends on third-party components. This document summarizes the prominent licenses you should be aware of when distributing or packaging this project. It is not legal advice — if you need legal certainty, consult a lawyer.

## This project's license

The code in this repository is licensed under the MIT License. See `LICENSE` at the repository root for the full text.

## Notable third-party dependencies included or referenced in this repository

1. OpenUSD (the USD project)
   - License: "Tomorrow Open Source Technology License 1.0" (TOMORROW OPEN SOURCE TECHNOLOGY LICENSE 1.0)
   - Location: `OpenUSD/LICENSE.txt`
   - Key points:
     - Grants broad copyright and patent licenses for use, modification and distribution.
     - Has specific trademark language (Section 6) that restricts use of OpenUSD trademarks.
     - Requires including the license text and preserving copyright and attribution notices when redistributing the Work or Derivative Works.
     - If the OpenUSD distribution includes a `NOTICE` file, derivative distributions should include the `NOTICE` contents as described in the license.
   - Practical implications:
     - You can distribute binaries that link against or include OpenUSD artifacts, but you must include the OpenUSD license text and any required notices in your distribution.
     - Do not make claims that would imply endorsement by the OpenUSD project or its trademarks.

2. godot-cpp (Godot C++ bindings)
   - License: MIT
   - Location: `godot-cpp/LICENSE.md`
   - Key points:
     - MIT is permissive and fully compatible with licensing this repository under MIT.
     - You must include the godot-cpp copyright and license text in distributions that include its code.

## Other bundled or referenced component licenses (from OpenUSD)

OpenUSD bundles or references many third-party libraries. Those components are licensed under a variety of licenses (Apache-2.0, MIT, BSD-family, public domain, etc.). A non-exhaustive list appears inside `OpenUSD/LICENSE.txt`.