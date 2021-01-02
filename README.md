# Info
Only tested on Ubuntu 18.x, no support for anything but that.

# Required Packages

## Ubuntu

Following packages/tools are needed to build the project (there may be more, but at least these):

`meson ninja libglfw3-dev libglew-dev libglm-dev libassimp-dev`

## Windows

Recommended to use `vcpkg` to manage your installed libraries. The following are the packages required

### Packages

`vcpkg install fmt:x64-windows glew:x64-windows glfw3:x64-windows spdlog:x64-windows glm:x64-windows assimp:x64-windows `

### vcpkg CMake integration

With `vcpkg` installed and integrated into system -- `vcpkg integrate install` -- simply supply the path to the cmake toolchain file.

This script should be located at: `${VC_ROOT}\scripts\buildsystems\vcpkg.cmake` where `${VC_ROOT}` is thte root directory of your `vcpkg` installation

To supply this to `cmake`, append `DCMAKE_TOOLCHAIN_FILE` to your `cmake -G` invocation. From there, build project how you wish: either through Visual Studio or by running `cmake --build .`