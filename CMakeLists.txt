# TraderHub/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(TraderHub)

# Set C++ standard for all projects
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Common package dependencies
find_package(nlohmann_json REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)

# Add common include directories
include_directories(${NLOHMANN_JSON_INCLUDE_DIRS})

# Build projects independently
add_subdirectory(TradingEngine)
add_subdirectory(OrderClientServer)

# Optional: Add documentation target
add_custom_target(docs
    COMMENT "Generating documentation..."
)
