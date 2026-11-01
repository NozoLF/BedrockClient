# Bedrock Workbench
A standalone Win32 offline prototype for ESP boxes, ore labels and radius-based target selection. Does not inject into Minecraft, read its memory, or bypass server controls.

## Build
Run `cmake -S . -B build -A x64` and `cmake --build build --config Release` on Windows with Visual Studio Build Tools.

## GitHub
Upload all files (including `.github/workflows/build.yml`) to a repository, then open Actions > Build Windows EXE > Run workflow. Download the artifact ZIP.

## Controls
F1 toggles ESP; F2 toggles ore markers; F3 toggles target selection; +/- adjusts range; Esc exits.
