# Info
Only tested on Ubuntu 18.x, no support for anything but that.

# Required Packages

## Ubuntu

Following packages/tools are needed to build the project (there may be more, but at least these):

`meson ninja libglfw3-dev libglew-dev libglm-dev libassimp-dev libfmt-dev libspdlog-dev`

## Windows

Recommended to use `vcpkg` to manage your installed libraries. The following are the packages required

### Packages

`vcpkg install fmt:x64-windows glew:x64-windows glfw3:x64-windows spdlog:x64-windows glm:x64-windows assimp:x64-windows `

### vcpkg CMake integration

With `vcpkg` installed and integrated into system -- `vcpkg integrate install` -- simply supply the path to the cmake toolchain file.

This script should be located at: `${VC_ROOT}\scripts\buildsystems\vcpkg.cmake` where `${VC_ROOT}` is thte root directory of your `vcpkg` installation

To supply this to `cmake`, append `DCMAKE_TOOLCHAIN_FILE` to your `cmake -G` invocation. From there, build project how you wish: either through Visual Studio or by running `cmake --build .`

# Rough Docs/Ideas

## Material System

The material system is split into two main ideas: Material Templates and Material Instances.

A Material template is defined as a collection of settings with either constant, or varying, values. For example, a very simple diffuse texture Material Template might have the following settings:

- Diffuse Texture
  - This is the texture for the color channel of the rendered object
  - This setting has a default setting pointing to the debug texture
  - User must specify the resource name, relative to the texture look-up location
- Lighting Enabled
  - Should this object receive lighting (and thus be shaded)?
  - This setting has a default setting which is `true`
  
A Material Instance, is a _copy_ of a Material Template. Any setting without a default value _must_ be resolved by the user of the material. Users should have a generic method of
looking up a setting by name and providing a value. Type checking must occur when setting the value.

Attempting to bind a material to the pipeline while required settings are not yet resolved yields an immediate trap.

`DrawableNode` will only accept Material Instances. Material Instances can be shared across multiple `DrawableNode`, with the understanding that modifying one will modify another
(typical pointer/ref behavior).

Material Instances should be `Cloneable`, which allows the creation of a new `MaterialInstance` -- with the current applied settings -- detached from the original `MaterialInstance`

`MaterialTemplate`s reside in the `resource` directory, under the `material-templates` sub-folder. They are `JSON`-encoded files.

`MaterialInstance`s reside in memory only. In the future, saving a `MaterialInstance` will be possible. Along with the ability to easily create new instances and modify their values
from an editor.