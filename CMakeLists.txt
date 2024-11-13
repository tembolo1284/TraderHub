# TraderHub/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(TraderHub)

# Set C++ standard for all projects
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find the nlohmann_json library
find_package(nlohmann_json REQUIRED)

# First build TradingEngine
add_subdirectory(TradingEngine)

# Then build OrderClientServer
# The OrderClientServer depends on TradingEngine
add_subdirectory(OrderClientServer)

# Add the include directories for the nlohmann_json library
include_directories(${NLOHMANN_JSON_INCLUDE_DIRS})
