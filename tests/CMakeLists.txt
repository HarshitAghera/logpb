cmake_minimum_required(VERSION 3.22.1) 

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

set(PROJECT_TESTS "${PROJECT_NAME}-tests")

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        b796f7d44681514f58a683a3a71ff17c94edb0c1  # release-1.13.0
)
FetchContent_MakeAvailable(googletest)

list(APPEND SOURCE_FILES
    logger_test.cpp
    session_test.cpp
)

add_executable(${PROJECT_TESTS} ${SOURCE_FILES})
target_link_libraries(${PROJECT_TESTS} PUBLIC gtest_main)
target_link_libraries(${PROJECT_TESTS} PUBLIC "${PROJECT_NAME}-app")
add_test(NAME tests COMMAND ${PROJECT_TESTS})
