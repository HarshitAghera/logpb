cmake_minimum_required(VERSION 3.22.1) 


add_library("${PROJECT_NAME}-app"
    # header files
    session.h
    session_loader.h

    #source files
    session.cpp
    session_loader.cpp
)

target_include_directories("${PROJECT_NAME}-app" PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries("${PROJECT_NAME}-app" PUBLIC "${PROJECT_NAME}-lib")
