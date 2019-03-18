add_executable(Nimble src/main.cpp)
target_compile_options(Nimble PRIVATE /W3 /WX)

target_link_libraries(Nimble PUBLIC NimbleGraphics)