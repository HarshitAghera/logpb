cmake_minimum_required(VERSION 3.22.1) 


add_library("${PROJECT_NAME}-app"
    # header files
    session.h

    #source files
    session.cpp
)

target_link_libraries("${PROJECT_NAME}-app" "${PROJECT_NAME}-lib")
