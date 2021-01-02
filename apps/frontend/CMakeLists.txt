add_executable(NimbleEngineFrontend src/main.cpp)
target_compile_features(NimbleEngineFrontend PUBLIC cxx_std_17)
set_target_properties(NimbleEngineFrontend PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(NimbleEngineFrontend PUBLIC NimbleEngine)

set_target_properties(NimbleEngineFrontend PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/install
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/install)

message("Copying resources to install")
# Install the resources folder next to the frontend post-build
add_custom_command(TARGET NimbleEngineFrontend POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:NimbleEngineFrontend>/resources
        )
