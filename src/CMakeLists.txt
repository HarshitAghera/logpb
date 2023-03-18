cmake_minimum_required(VERSION 3.22.1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/module")
include(AddCXXCompilerFlag)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
else ()
add_cxx_compiler_flag(-Wall)
add_cxx_compiler_flag(-Wextra)
add_cxx_compiler_flag(-Wsuggest-override)
add_cxx_compiler_flag(-pedantic)
add_cxx_compiler_flag(-pedantic-errors)
add_cxx_compiler_flag(-Wshorten-64-to-32)
add_cxx_compiler_flag(-Wthread-safety)

add_cxx_compiler_flag(-Wno-shadow)
add_cxx_compiler_flag(-Wno-unused-parameter)
add_cxx_compiler_flag(-Wno-narrowing)
add_cxx_compiler_flag(-Wno-unused-variable)
add_cxx_compiler_flag(-Wno-missing-field-initializers)
add_cxx_compiler_flag(-Wno-lto-type-mismatch)
add_cxx_compiler_flag(-Wno-pointer-arith)

add_cxx_compiler_flag(-fdiagnostics-color=always)
add_cxx_compiler_flag(-fsanitize=address)
add_cxx_compiler_flag(-fstrict-aliasing)
# add_cxx_compiler_flag(-flto)

add_cxx_compiler_flag(-fno-exceptions)
add_cxx_compiler_flag(-fno-rtti)

add_cxx_compiler_flag(-static)
endif()


add_subdirectory(base)

add_subdirectory(app)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

else ()
    list(APPEND CMAKE_PREFIX_PATH "/home/harshit/Qt/6.3.2/gcc_64")
endif ()

add_subdirectory(gui)
