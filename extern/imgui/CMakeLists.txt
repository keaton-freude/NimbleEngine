add_library(ImGui
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_impl_glfw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_tables.cpp
        )

target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ImGui PRIVATE glfw GLEW::GLEW)