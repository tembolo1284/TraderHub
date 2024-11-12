# Trading Engine

A high-performance C++20 trading engine implementation with price-time priority matching.

## Features

- Price-time priority order matching
- Support for market and limit orders
- Partial order fills
- Order cancellation
- Real-time order book management
- Thread-safe operations
- Comprehensive test suite (unit, integration, performance)

## Requirements

- C++20 compatible compiler (GCC 10+, Clang 10+, or MSVC 2019+)
- CMake 3.15 or higher
- Ninja build system (recommended)
- Google Test (automatically fetched by CMake)

### Ubuntu/Debian

```bash
sudo apt update
sudo apt install build-essential cmake ninja-build
```

### macOS

```bash
brew install cmake ninja
```

### Windows

```bash
winget install Kitware.CMake
winget install Ninja-build.Ninja
```

## Project Structure

```
trading-engine/
├── include/                    # Header files
│   ├── order.hpp
│   ├── order_book.hpp
│   ├── prioritizable_value_st.hpp
│   ├── trade.hpp
│   └── trader.hpp
├── src/                       # Implementation files
│   ├── order.cpp
│   ├── order_book.cpp
│   ├── trade.cpp
│   └── trader.cpp
├── tests/                     # Test files
│   ├── unit/                  # Unit tests
│   ├── integration/           # Integration tests
│   └── performance/          # Performance tests
├── CMakeLists.txt            # Build configuration
└── README.md                 # This file
```

## Building the Project

Clone the repository:
```bash
git clone <url>
cd trading-engine
```

### Debug Build

```bash
# Configure with Ninja (recommended)
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build
```

### Release Build

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Running Tests

From the build directory:

### Run All Tests
```bash
cd build
ctest --output-on-failure
```

### Run Specific Test Categories

```bash
# Unit Tests
./build/unit_tests

# Integration Tests
./build/integration_tests

# Performance Tests
./build/performance_tests
```

### Run Tests with More Detail
```bash
# Run with colored output
./build/unit_tests --gtest_color=yes

# Run specific tests
./build/unit_tests --gtest_filter=OrderTest.*

# Run with verbose output
./build/unit_tests -V

# List all available tests
./build/unit_tests --gtest_list_tests
```

### Using Custom Targets

```bash
# Run unit tests
cmake --build build --target run_unit_tests

# Run integration tests
cmake --build build --target run_integration_tests

# Run performance tests
cmake --build build --target run_performance_tests

# Run all tests
cmake --build build --target run_all_tests
```

## Core Components

- **Order**: Represents a trading order with price-time priority
- **OrderBook**: Manages and matches buy/sell orders
- **Trade**: Represents an executed trade between two orders
- **Trader**: Represents a market participant
- **PrioritizableValueST**: Custom data structure for efficient order management

## Implementation Details

### Price-Time Priority

Orders are matched based on:
1. Price priority (best price first)
2. Time priority (first-in-first-out at same price)

### Order Matching

- Buy orders are matched with sell orders when prices cross
- Partial fills are supported
- Orders can be cancelled at any time
- Matches occur at the sell order's price

## Code Style

The project uses strict C++20 standards and enforces:
- Comprehensive warnings enabled
- Warnings treated as errors
- Static analysis
- Clang-format for consistent style
- AddressSanitizer and UndefinedBehaviorSanitizer in Debug builds

## Performance

The trading engine is designed for high-performance operation:
- O(log n) order insertion and deletion
- O(1) order lookup
- Efficient partial fill handling
- Memory-efficient data structures

