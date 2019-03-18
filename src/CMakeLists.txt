add_library(NimbleGraphics STATIC
opengl-wrapper/FragmentShader.cpp
opengl-wrapper/glad.c
opengl-wrapper/IndexBuffer.cpp
Mesh.cpp
opengl-wrapper/Shader.cpp
opengl-wrapper/ShaderProgram.cpp
opengl-wrapper/VertexShader.cpp
opengl-wrapper/VertexBuffer.cpp
window/Window.cpp)

target_include_directories(NimbleGraphics PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_include_directories(NimbleGraphics PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/)

# link to required external dependencies
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(NimbleGraphics PUBLIC glfw)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(NimbleGraphics PUBLIC fmt::fmt)

find_package(OpenGL)
target_link_libraries(NimbleGraphics PUBLIC ${OPENGL_LIBRARIES})