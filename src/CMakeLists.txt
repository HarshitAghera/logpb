cmake_minimum_required(VERSION 3.22.1) 

add_subdirectory(base)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

else ()
    list(APPEND CMAKE_PREFIX_PATH "/home/harshit/Qt/6.3.2/gcc_64")
endif ()

add_subdirectory(gui)

add_subdirectory(app)
