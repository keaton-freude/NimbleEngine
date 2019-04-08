add_library(NimbleGraphics STATIC
opengl-wrapper/FragmentShader.cpp
opengl-wrapper/IndexBuffer.cpp
Mesh.cpp
opengl-wrapper/Shader.cpp
opengl-wrapper/ShaderProgram.cpp
opengl-wrapper/VertexShader.cpp
opengl-wrapper/VertexBuffer.cpp
window/Window.cpp)

# Disgusting CMake nonsense to enable all warnings for each compiler toolchain, and makes warnings into errors (MSVC not tested)
target_compile_options(NimbleGraphics PRIVATE
                           $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
                           -Wall -Werror -pedantic-errors -Wextra -Wconversion -Wsign-conversion>
                           $<$<CXX_COMPILER_ID:MSVC>:
                                /W3>)

# gcc/clang only, msvc equivalent somewhere?
target_compile_options(NimbleGraphics PRIVATE -fmax-errors=5)

target_include_directories(NimbleGraphics PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(NimbleGraphics PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/)

# link to required external dependencies
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(NimbleGraphics PUBLIC glfw)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(NimbleGraphics PUBLIC fmt::fmt)

find_package(OpenGL)
target_link_libraries(NimbleGraphics PUBLIC ${OPENGL_LIBRARIES})

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
        target_link_libraries(NimbleGraphics PUBLIC GLEW)
endif()
