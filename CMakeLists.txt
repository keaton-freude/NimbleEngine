cmake_minimum_required(VERSION 3.11)

project(NimbleEngine VERSION 0.0.1 LANGUAGES CXX)

set(MSVC_COMPILE_FLAGS "/MP")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MSVC_COMPILE_FLAGS}")

# Find all of our dependencies up front
# NOTE: If you are on windows, install vcpkg and install your libaries first
# and use the build script or remember to add the vcpkg to your cmake cmd line
find_package(OpenGL REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
add_subdirectory(extern/imgui)


# Nimble Engine
add_library(NimbleEngine
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Mesh.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/opengl-wrapper/FragmentShader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/opengl-wrapper/GLContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/opengl-wrapper/IndexBuffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/opengl-wrapper/Shader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/opengl-wrapper/ShaderProgram.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/opengl-wrapper/VertexBuffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/opengl-wrapper/VertexShader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/window/Window.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/resource-manager/ResourceManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/RenderLoop.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/Engine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/GlfwRenderLoop.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/input/InputManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/material/Material.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/camera/Camera.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/scene-graph/SceneNode.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/scene-graph/SceneGraph.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/opengl-wrapper/Texture2D.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/scene-graph/RootSceneNode.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/scene-graph/Transformation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/scene-graph/DirectionalLightNode.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/opengl-wrapper/Sampler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/subsystem/Subsystem.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/subsystem/FileWatcherSubsystem.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stb_stub.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/implot-0.3/implot.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/implot-0.3/implot_demo.cpp
        )
target_compile_features(NimbleEngine PUBLIC cxx_std_17)
set_target_properties(NimbleEngine PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(NimbleEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(NimbleEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR}/extern/implot-0.3)
target_include_directories(NimbleEngine PRIVATE ${CMAKE_CURRENT_LIST_DIR}/extern/simdjson/include)
target_include_directories(NimbleEngine PRIVATE ${CMAKE_CURRENT_LIST_DIR}/extern/stb)
add_subdirectory(extern/simdjson EXCLUDE_FROM_ALL)
target_link_libraries(NimbleEngine PUBLIC ImGui)
target_link_libraries(NimbleEngine PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(NimbleEngine PRIVATE fmt::fmt)
target_link_libraries(NimbleEngine PUBLIC GLEW::GLEW)
target_link_libraries(NimbleEngine PUBLIC glfw)
target_link_libraries(NimbleEngine PRIVATE spdlog::spdlog)
target_link_libraries(NimbleEngine PRIVATE glm)
target_link_libraries(NimbleEngine PRIVATE assimp::assimp)
target_link_libraries(NimbleEngine PRIVATE simdjson)
# Front end app
add_subdirectory(apps)
add_subdirectory(tests/nimble)