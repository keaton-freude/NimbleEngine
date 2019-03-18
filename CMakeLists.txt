cmake_minimum_required(VERSION 3.1...3.13)

if (${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Nimble VERSION 0.1.0
    DESCRIPTION "Nimble Rendering Engine"
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_subdirectory(frontend)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
