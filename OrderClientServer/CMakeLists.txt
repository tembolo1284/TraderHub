# OrderClientServer/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(OrderClientServer)

# Find dependencies
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/TradingEngine/include  # Changed from relative path to use CMAKE_SOURCE_DIR
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

# Core library for OrderClientServer
add_library(OrderClientServerLib
    src/order_client_server.cpp
    src/order_service.cpp
    ${GENERATED_PROTO_SRCS}
)

# Set include directories and link libraries for OrderClientServerLib
target_include_directories(OrderClientServerLib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/TradingEngine/include
)

target_link_libraries(OrderClientServerLib
    PUBLIC
        TradingEngineLib  # Link to TradingEngine library
        gRPC::grpc++
        protobuf::libprotobuf
        spdlog::spdlog
    PRIVATE
        gRPC::grpc
)

# Executables
add_executable(OrderServer src/main.cpp)
target_link_libraries(OrderServer PRIVATE OrderClientServerLib)

add_executable(OrderClient src/order_client_main.cpp ${GENERATED_PROTO_SRCS})
target_link_libraries(OrderClient
    PRIVATE
        protobuf::libprotobuf
        gRPC::grpc++
        gRPC::grpc
        spdlog::spdlog
        nlohmann_json::nlohmann_json
)

# Test executable
add_executable(OrderClientServerTests tests/order_client_server_tests.cpp)
target_link_libraries(OrderClientServerTests
    PRIVATE
        OrderClientServerLib
        GTest::GTest 
        GTest::Main
)

# Enable testing
enable_testing()
add_test(NAME OrderClientServerTests COMMAND OrderClientServerTests)

# Installation rules
install(TARGETS 
    OrderServer
    OrderClient
    OrderClientServerLib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)
