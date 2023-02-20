cmake_minimum_required(VERSION 3.22.1) 

list(APPEND SOURCE_FILES
    msg_type_gen.cpp
    logger.cpp
    stream_parser.cpp
    stream_generator.cpp
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

# protobuf lib
add_subdirectory(third-party/protobuf-3.21.12)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/module")
include(AddCXXCompilerFlag)

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

add_cxx_compiler_flag(-fdiagnostics-color=always)
add_cxx_compiler_flag(-fsanitize=address)
add_cxx_compiler_flag(-fstrict-aliasing)
# add_cxx_compiler_flag(-flto)

add_cxx_compiler_flag(-fno-exceptions)
add_cxx_compiler_flag(-fno-rtti)

add_cxx_compiler_flag(-static)


set(PROJECT_LIB "${PROJECT_NAME}-lib")

add_library(${PROJECT_LIB} ${SOURCE_FILES})
target_link_libraries(${PROJECT_LIB} ${CONAN_LIBS})
target_link_libraries(${PROJECT_LIB} libprotobuf)
target_include_directories(${PROJECT_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIB})
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"

    COMMAND ${CMAKE_COMMAND} -E
    copy ${CMAKE_CURRENT_SOURCE_DIR}/../resources/nanopb.proto "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
         ${CMAKE_CURRENT_SOURCE_DIR}/third-party/protobuf-3.21.12/src/google/protobuf/descriptor.proto
         "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
    )
