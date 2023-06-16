cmake_minimum_required(VERSION 3.22.1) 

list(APPEND SOURCE_FILES
    msg_type_gen.cpp
    logger.cpp
    stream_parser.cpp
    stream_generator.cpp
    parse_error.cpp
    device_connection.cpp
    device_connection_impl.cpp
    plotter.cpp

    # headers
    msg_type_gen.h
    logger.h
    stream_parser.h
    stream_generator.h
    parse_error.h
    device_connection.h
    device_connection_impl.h
    plotter.h
)

# auto generated protobuf message definitions
 list(APPEND SOURCE_FILES
     msg_defs/nanopb.pb.cc
     msg_defs/msg_defs.pb.cc)

# protobuf lib options
option(protobuf_INSTALL "Install protobuf binaries and files" OFF)
option(protobuf_BUILD_TESTS "Build tests" OFF)
option(WITH_PROTOC "Build protoc" OFF)
option(protobuf_BUILD_TESTS "Build tests" OFF)
option(protobuf_BUILD_CONFORMANCE "Build conformance tests" OFF)
option(protobuf_BUILD_EXAMPLES "Build examples" OFF)
option(protobuf_BUILD_PROTOC_BINARIES "Build libprotoc and protoc compiler" OFF)
option(protobuf_BUILD_LIBPROTOC "Build libprotoc" OFF)
option(protobuf_DISABLE_RTTI "Remove runtime type information in the binaries" OFF)
option(protobuf_MSVC_STATIC_RUNTIME OFF)


# protobuf lib
add_subdirectory(third-party/protobuf-3.21.12)

set(PROJECT_LIB "${PROJECT_NAME}-lib")

add_library(${PROJECT_LIB} ${SOURCE_FILES})
target_link_libraries(${PROJECT_LIB} ${CONAN_LIBS})
target_link_libraries(${PROJECT_LIB} libprotobuf)
target_include_directories(${PROJECT_LIB} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

set(PROJECT_NAME_BASE "${PROJECT_NAME}-base")
add_executable(${PROJECT_NAME_BASE} main.cpp) 
target_link_libraries(${PROJECT_NAME_BASE} ${PROJECT_LIB})
add_custom_command(
    TARGET ${PROJECT_NAME_BASE} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME_BASE}>/resources"

    COMMAND ${CMAKE_COMMAND} -E
    copy ${CMAKE_SOURCE_DIR}/resources/nanopb.proto "$<TARGET_FILE_DIR:${PROJECT_NAME_BASE}>/resources"
         ${CMAKE_CURRENT_SOURCE_DIR}/third-party/protobuf-3.21.12/src/google/protobuf/descriptor.proto
         "$<TARGET_FILE_DIR:${PROJECT_NAME_BASE}>/resources"
    )
