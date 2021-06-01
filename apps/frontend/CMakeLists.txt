add_executable(NimbleEngineFrontend src/main.cpp)
target_compile_features(NimbleEngineFrontend PUBLIC cxx_std_17)
set_target_properties(NimbleEngineFrontend PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(NimbleEngineFrontend PUBLIC NimbleEngine)

set_target_properties(NimbleEngineFrontend PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/install
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/install)

message("LeakCheck: ${NIMBLE_LEAK_CHECK}")

if(${NIMBLE_LEAK_CHECK})
    message("Leak checking enabled")
    target_include_directories(NimbleEngineFrontend PRIVATE ${PROJECT_SOURCE_DIR}/extern/vld/include)
    target_link_libraries(NimbleEngineFrontend PRIVATE ${PROJECT_SOURCE_DIR}/extern/vld/lib/Win64/vld.lib)
else()
    message("Leak checking disabled")
endif()


message("Copying resources to install")
# Install the resources folder next to the frontend post-build
add_custom_command(TARGET NimbleEngineFrontend POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:NimbleEngineFrontend>/resources
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/extern/vld/bin/Win64/vld_x64.dll $<TARGET_FILE_DIR:NimbleEngineFrontend>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/extern/vld/bin/Win64/dbghelp.dll $<TARGET_FILE_DIR:NimbleEngineFrontend>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/extern/vld/bin/Win64/Microsoft.DTfW.DHL.manifest $<TARGET_FILE_DIR:NimbleEngineFrontend>
        )
