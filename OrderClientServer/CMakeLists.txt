cmake_minimum_required(VERSION 3.10)
project(OrderClientServer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find all required dependencies
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)  # Add spdlog dependency

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../TradingEngine/include
    ${spdlog_INCLUDE_DIRS}
)

# Proto files configuration
set(PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/protos/order_service.proto)
set(GENERATED_PROTO_SRCS 
    "${CMAKE_CURRENT_BINARY_DIR}/order_service.pb.cc"
    "${CMAKE_CURRENT_BINARY_DIR}/order_service.grpc.pb.cc"
)
set(GENERATED_PROTO_HDRS 
    "${CMAKE_CURRENT_BINARY_DIR}/order_service.pb.h"
    "${CMAKE_CURRENT_BINARY_DIR}/order_service.grpc.pb.h"
)

# Find gRPC plugin
find_program(gRPC_CPP_PLUGIN grpc_cpp_plugin PATHS /usr/local/bin)

# Generate Protobuf and gRPC files
add_custom_command(
    OUTPUT ${GENERATED_PROTO_SRCS} ${GENERATED_PROTO_HDRS}
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
    ARGS --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
         --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
         --plugin=protoc-gen-grpc=${gRPC_CPP_PLUGIN}
         -I ${CMAKE_CURRENT_SOURCE_DIR}/protos
         ${PROTO_FILES}
    DEPENDS ${PROTO_FILES}
)

# TradingEngine sources
set(TRADING_ENGINE_SOURCES
    ../TradingEngine/src/order.cpp
    ../TradingEngine/src/order_book.cpp
    ../TradingEngine/src/prioritizable_value_st.cpp
    ../TradingEngine/src/trade.cpp
    ../TradingEngine/src/trader.cpp
)

# OrderClientServer sources
set(ORDER_CLIENT_SERVER_SOURCES
    src/order_client_server.cpp
    src/order_service.cpp
    ${GENERATED_PROTO_SRCS}
)

# Create core library
add_library(OrderClientServerLib
    ${ORDER_CLIENT_SERVER_SOURCES}
    ${TRADING_ENGINE_SOURCES}
)

# Set properties for the library
set_target_properties(OrderClientServerLib PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Library include directories and dependencies
target_include_directories(OrderClientServerLib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../TradingEngine/include
)

target_link_libraries(OrderClientServerLib
    PUBLIC
        gRPC::grpc++
        protobuf::libprotobuf
        spdlog::spdlog
    PRIVATE
        gRPC::grpc
)

# Server executable
add_executable(OrderServer src/main.cpp)
target_link_libraries(OrderServer 
    PRIVATE 
        OrderClientServerLib
        spdlog::spdlog
)

# Tests configuration
add_executable(OrderClientServerTests 
    tests/order_client_server_tests.cpp
)

target_link_libraries(OrderClientServerTests
    PRIVATE
        OrderClientServerLib
        GTest::GTest 
        GTest::Main
        spdlog::spdlog
)

add_executable(OrderClient src/order_client_main.cpp ${GENERATED_PROTO_SRCS})
target_link_libraries(OrderClient
    PRIVATE
        protobuf::libprotobuf
        gRPC::grpc++
        gRPC::grpc
)

# Enable testing
enable_testing()
add_test(NAME OrderClientServerTests COMMAND OrderClientServerTests)

# Compiler warnings
if(MSVC)
    target_compile_options(OrderClientServerLib PRIVATE /W4)
    target_compile_options(OrderServer PRIVATE /W4)
    target_compile_options(OrderClientServerTests PRIVATE /W4)
else()
    target_compile_options(OrderClientServerLib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(OrderServer PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(OrderClientServerTests PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Installation rules
install(TARGETS OrderServer
    RUNTIME DESTINATION bin
)

install(TARGETS OrderClientServerLib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)
