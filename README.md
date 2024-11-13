# TraderHub

A trading system consisting of a Trading Engine with price-time priority matching and an Order Client Server for handling order submissions.

## Project Structure

TraderHub/

├── TradingEngine/       # Core matching engine implementation

├── OrderClientServer/   # Client-server interface for order management

└── build/              # Build outputs (created during build)

## Prerequisites
- CMake (>= 3.15)
- C++ Compiler supporting C++20
- Ninja build system
- gRPC
- Protocol Buffers
- GTest
- spdlog
- nlohmann_json

## Building the Project

From the root TraderHub directory:
```bash
# Configure the build
cmake -B build -G Ninja

# Build all targets
cmake --build build

## Running the Applications

### Order Client Server

## From the build directory:

## Start the server
./OrderClientServer/OrderServer

## In another terminal, use the client
./OrderClientServer/OrderClient

## Client commands:
./OrderClientServer/OrderClient submit <order_id> <trader_id> <symbol> <price> <quantity> <buy/sell>
./OrderClientServer/OrderClient cancel <order_id> <buy/sell>
./OrderClientServer/OrderClient view [symbol]
./OrderClientServer/OrderClient file <filename>

## Examples:
./OrderClientServer/OrderClient submit order1 trader1 AAPL 150.50 100 buy
./OrderClientServer/OrderClient cancel order1 buy
./OrderClientServer/OrderClient view AAPL

## Running Tests

## From the build directory:

# Trading Engine Tests

cd TradingEngine
ctest --output-on-failure

# Or run test executables directly:
./unit_tests
./integration_tests
./performance_tests

# Order Client Server Tests

cd OrderClientServer
ctest --output-on-failure

# Or run directly:
./OrderClientServerTests

# Project Components

## Trading Engine

- Price-time priority matching engine

- Order book management

- Trade execution

- Position tracking

## Order Client Server

- gRPC-based client-server architecture

- Order submission and cancellation

- Order book viewing

- JSON file-based order processing

# Development

## Adding New Tests

- Unit tests go in `TradingEngine/tests/unit` or `OrderClientServer/tests/`

- Integration tests in `TradingEngine/tests/integration/`

- Performance tests in `TradingEngine/tests/performance/`

# Building for Debug

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

# Building for Release

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

