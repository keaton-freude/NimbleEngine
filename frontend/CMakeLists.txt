add_executable(Nimble src/main.cpp)

# Disgusting CMake nonsense to enable all warnings for each compiler toolchain, and makes warnings into errors (MSVC not tested)
target_compile_options(Nimble PRIVATE
                           $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
                           -Wall -Werror -pedantic-errors -Wextra -Wconversion -Wsign-conversion>
                           $<$<CXX_COMPILER_ID:MSVC>:
                                /W3>)

target_compile_options(Nimble PRIVATE -fmax-errors=5)
target_link_libraries(Nimble PUBLIC NimbleGraphics)
