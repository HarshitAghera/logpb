cmake_minimum_required(VERSION 3.22.1)

project(logpb VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_IMPORT_LIBRARY_PREFIX "")
set(CMAKE_COLOR_DIAGNOSTICS ON)

include(${PROJECT_BINARY_DIR}/conanbuildinfo.cmake)  # Includes the contents of the conanbuildinfo.cmake file.
conan_basic_setup()  # Prepares the CMakeList.txt for Conan.

add_subdirectory(src)

set(PROJECT_BUILD_TESTS TRUE CACHE BOOL "")

if(PROJECT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
