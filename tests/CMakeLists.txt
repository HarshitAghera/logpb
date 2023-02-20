cmake_minimum_required(VERSION 3.22.1) 

set(PROJECT_TESTS "${PROJECT_NAME}-tests")

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        b796f7d44681514f58a683a3a71ff17c94edb0c1  # release-1.13.0
)
FetchContent_MakeAvailable(googletest)

list(APPEND SOURCE_FILES
    logger-test.cpp
)

add_executable(${PROJECT_TESTS} ${SOURCE_FILES})
target_link_libraries(${PROJECT_TESTS} gtest_main)
add_test(NAME tests COMMAND ${PROJECT_TESTS})
