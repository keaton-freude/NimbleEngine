cmake_minimum_required(VERSION 3.5)

project(NimbleTests LANGUAGES CXX VERSION 0.0.1)

find_package(Catch2 REQUIRED)
add_executable(NimbleTests
        ${CMAKE_CURRENT_LIST_DIR}/scene-graph.cpp
        ${CMAKE_CURRENT_LIST_DIR}/subject.cpp
        )

target_link_libraries(NimbleTests PRIVATE Catch2::Catch2 NimbleEngine)
target_link_libraries(NimbleTests PUBLIC NimbleEngine)
target_include_directories(NimbleTests PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

include(CTest)
include(Catch)
catch_discover_tests(NimbleTests)