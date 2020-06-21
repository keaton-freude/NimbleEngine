cmake_minimum_required(VERSION 3.11)

project(NimbleEngine VERSION 0.0.1 LANGUAGES CXX)

set(MSVC_COMPILE_FLAGS "/MP")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MSVC_COMPILE_FLAGS}" )

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

add_subdirectory(src)
add_subdirectory(extern/imgui)


# Nimble Engine
add_library(NimbleEngine ${NIMBLE_ENGINE_SOURCES})
target_compile_features(NimbleEngine PUBLIC cxx_std_17)
set_target_properties(NimbleEngine PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(NimbleEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(NimbleEngine PUBLIC ImGui)
target_link_libraries(NimbleEngine PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(NimbleEngine PRIVATE fmt::fmt)
target_link_libraries(NimbleEngine PUBLIC GLEW::GLEW)
target_link_libraries(NimbleEngine PUBLIC glfw)
target_link_libraries(NimbleEngine PRIVATE spdlog::spdlog)
target_link_libraries(NimbleEngine PRIVATE glm)
target_link_libraries(NimbleEngine PRIVATE assimp::assimp)



# Front end app
add_subdirectory(apps)