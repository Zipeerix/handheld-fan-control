cmake_minimum_required(VERSION 3.20)

project(
    HandheldFanControlBackend
    VERSION 1.0
    LANGUAGES CXX)

find_package(protobuf REQUIRED)
find_package(Boost REQUIRED)
find_package(tomlplusplus REQUIRED)
find_package(spdlog REQUIRED)
find_package(libenvpp REQUIRED)
find_package(GTest REQUIRED)

if(SKIP_STATIC_ANALYSIS)
    message("Skipping static analysis")
else()
    set(CMAKE_CXX_CLANG_TIDY clang-tidy; -format-style='file';)
endif()

set(PROTO_FILES proto/FanControlService.proto)
set(APPLICATION_SOURCE_FILES
    src/daemon/UnixDomainServer.cpp
    src/daemon/UnixDomainServer.hpp
    src/daemon/DaemonServer.cpp
    src/daemon/DaemonServer.hpp
    src/bin/ConfigParser.cpp
    src/bin/ConfigParser.hpp
    src/utils/Logging.cpp
    src/utils/Time.cpp
    src/utils/Time.hpp
    src/utils/Logging.hpp
    src/meta/Metadata.cpp
    src/meta/Metadata.hpp
    src/daemon/RequestProcessor.cpp
    src/daemon/RequestProcessor.hpp
    src/core/FanController.cpp
    src/core/ProtoConversion.cpp
    src/core/ProtoConversion.hpp
    src/daemon/RequestIdGenerator.cpp
    src/daemon/RequestIdGenerator.hpp
    src/utils/Environment.cpp
    src/utils/Environment.hpp
    src/core/FanController.hpp
    src/core/ApplicationCoreContext.hpp
    src/daemon/FanControlServiceServer.cpp
    src/daemon/FanControlServiceServer.hpp
    src/utils/ProtobufTools.cpp
    src/utils/ProtobufTools.hpp)

set(TEST_FILES
    test/main.cpp
    test/tests/meta/Metadata.Tests.cpp
    test/tests/meta/Error.Tests.cpp
    test/tests/utils/ProtobufTools.Tests.cpp)

set(COMMON_LIBS_TO_LINK
    HandheldFanControlBackend_Proto
    boost::boost
    spdlog::spdlog
    libenvpp::libenvpp
    tomlplusplus::tomlplusplus
    protobuf::protobuf)
set(TEST_LIBS_TO_LINK gtest::gtest)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
add_library(HandheldFanControlBackend_Proto ${PROTO_SRCS} ${PROTO_HDRS})
target_include_directories(HandheldFanControlBackend_Proto PUBLIC ${Protobuf_INCLUDE_DIRS})
target_include_directories(HandheldFanControlBackend_Proto PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(HandheldFanControlBackend_Proto PUBLIC protobuf::protobuf)

add_executable(HandheldFanControlBackend src/bin/main.cpp ${APPLICATION_SOURCE_FILES})
set_target_properties(
    HandheldFanControlBackend
    PROPERTIES CXX_STANDARD 23
               CXX_STANDARD_REQUIRED YES
               CXX_EXTENSIONS OFF)
target_compile_definitions(HandheldFanControlBackend
                           PRIVATE -DHANDHELD_FAN_CONTROL_VERSION="${HANDHELD_FAN_CONTROL_VERSION}")

target_link_libraries(HandheldFanControlBackend PRIVATE ${COMMON_LIBS_TO_LINK})
target_include_directories(HandheldFanControlBackend PRIVATE src)
target_include_directories(HandheldFanControlBackend PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

if(SKIP_COMPILER_FLAGS)
    message("Skipping compiler flags")
else()
    target_compile_options(
        HandheldFanControlBackend
        PUBLIC "-Wall"
               "-Wextra"
               "-Werror"
               "-Wshadow"
               "-Wundef"
               "-Wcast-align"
               "-Wpointer-arith"
               "-Wwrite-strings"
               "-Wcast-qual"
               "-Wswitch-enum"
               "$<$<CONFIG:RELEASE>:-O3>")
endif()

if(SKIP_TESTS)
    message("Skipping tests")
else()
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test/resources)
        file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test/resources
             DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test)
    endif()

    protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS test/proto/ProtoForTests.proto)
    add_library(HandheldFanControlBackend_InternalProtoForTests ${PROTO_SRCS} ${PROTO_HDRS})
    target_link_libraries(HandheldFanControlBackend_InternalProtoForTests
                          PRIVATE protobuf::protobuf)
    target_include_directories(HandheldFanControlBackend_InternalProtoForTests
                               PRIVATE ${Protobuf_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

    add_executable(HandheldFanControlBackend_Tests ${TEST_FILES} ${APPLICATION_SOURCE_FILES})

    target_link_libraries(
        HandheldFanControlBackend_Tests PRIVATE HandheldFanControlBackend_InternalProtoForTests
                                                ${TEST_LIBS_TO_LINK} ${COMMON_LIBS_TO_LINK})

    set_target_properties(
        HandheldFanControlBackend_Tests
        PROPERTIES CXX_STANDARD 23
                   CXX_STANDARD_REQUIRED YES
                   CXX_EXTENSIONS OFF)
    target_compile_definitions(
        HandheldFanControlBackend_Tests
        PRIVATE -DHANDHELD_FAN_CONTROL_VERSION="${HANDHELD_FAN_CONTROL_VERSION}")
    target_include_directories(HandheldFanControlBackend_Tests PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

    target_include_directories(HandheldFanControlBackend_Tests PRIVATE src)
    target_include_directories(HandheldFanControlBackend_Tests PRIVATE test)
endif()
