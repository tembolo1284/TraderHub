# TraderHub
A trading system consisting of a Trading Engine with price-time priority matching and an Order Client Server for handling order submissions.

## Project Structure
```

TraderHub/

├── TradingEngine/       # Core matching engine implementation

├── OrderClientServer/   # Client-server interface for order management

└── build/              # Build outputs (created during build)
```

## Prerequisites
- CMake (>= 3.15)
- C++ Compiler supporting C++20
- Ninja build system
- gRPC
- Protocol Buffers
- GTest
- spdlog
- nlohmann_json
- Docker (optional, for containerized deployment)

## Installing Dependencies

### Basic Build Tools
```bash
# Update package list and install basic build tools
sudo apt update
sudo apt install -y build-essential cmake ninja-build
```

### Installing spdlog
```bash
# Option 1: Install from package manager (Ubuntu)
sudo apt install libspdlog-dev

# Option 2: Build from source
git clone https://github.com/gabime/spdlog.git
cd spdlog
mkdir build && cd build
cmake ..
sudo make install
```

### Installing gRPC and Protocol Buffers
```bash
# Install dependencies
sudo apt update
sudo apt install -y build-essential autoconf libtool pkg-config

# Clone and install gRPC
git clone --recurse-submodules -b v1.42.0 https://github.com/grpc/grpc
cd grpc
mkdir -p cmake/build
cd cmake/build
cmake ../.. -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local
make -j
sudo make install
```

### Installing GTest
```bash
# Install from package manager
sudo apt install libgtest-dev

# Build and install
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp lib/*.a /usr/lib
```

### Installing nlohmann_json
```bash
# Install from package manager
sudo apt install nlohmann-json3-dev
```

## Building and Running

### Option 1: Building Locally
From the root TraderHub directory:
```bash
# Configure the build
cmake -B build -G Ninja

# Build all targets
cmake --build build
```

### Option 2: Using Docker (Recommended)
The project includes Docker support for easy deployment and testing. Make sure you have Docker and Docker Compose installed.

```bash
# Make the run script executable
chmod +x docker-run.sh

# Build the Docker images
./docker-run.sh build

# Start the server (in one terminal)
./docker-run.sh server

# In another terminal, run client commands:
./docker-run.sh client view                                    # View all orders
./docker-run.sh client view AAPL                              # View AAPL orders
./docker-run.sh client submit order1 trader1 AAPL 150.50 100 buy  # Submit order
./docker-run.sh client cancel order1 buy                      # Cancel order
./docker-run.sh client file orders.json                       # Process orders from file

# Clean up containers and networks when done
./docker-run.sh clean
```

## Running the Applications

### Option 1: Running Locally
From the build directory:
```bash
# Start the server
./OrderClientServer/OrderServer

# In another terminal, use the client
./OrderClientServer/OrderClient
```

### Client Commands (Local Mode)
```bash
./OrderClientServer/OrderClient submit <order_id> <trader_id> <symbol> <price> <quantity> <buy/sell>
./OrderClientServer/OrderClient cancel <order_id> <buy/sell>
./OrderClientServer/OrderClient view [symbol]
./OrderClientServer/OrderClient file <filename>
```

### Examples (Local Mode)
```bash
./OrderClientServer/OrderClient submit order1 trader1 AAPL 150.50 100 buy
./OrderClientServer/OrderClient cancel order1 buy
./OrderClientServer/OrderClient view AAPL
```

## Running Tests
From the build directory:

### Trading Engine Tests
```bash
cd TradingEngine
ctest --output-on-failure

# Or run test executables directly:
./unit_tests
./integration_tests
./performance_tests
```

### Order Client Server Tests
```bash
cd OrderClientServer
ctest --output-on-failure

# Or run directly:
./OrderClientServerTests
```

## Project Components

### Trading Engine
- Price-time priority matching engine
- Order book management
- Trade execution
- Position tracking

### Order Client Server
- gRPC-based client-server architecture
- Order submission and cancellation
- Order book viewing
- JSON file-based order processing

## Development

### Adding New Tests
- Unit tests go in `TradingEngine/tests/unit` or `OrderClientServer/tests/`
- Integration tests in `TradingEngine/tests/integration/`
- Performance tests in `TradingEngine/tests/performance/`

### Building for Debug
```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Building for Release
```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Troubleshooting

### Common CMake Issues
If CMake can't find a package, try adding the installation directory to CMAKE_PREFIX_PATH:
```bash
cmake -B build -G Ninja -DCMAKE_PREFIX_PATH=/usr/local
```

If you encounter permission issues during installation, make sure you have the necessary rights:
```bash
sudo ldconfig
```

### Docker Issues
If the client can't connect to the server:
1. Ensure the server container is running (`docker ps`)
2. Wait a few seconds after starting the server before running client commands
3. If issues persist, try cleaning up and rebuilding:
```bash
./docker-run.sh clean
./docker-run.sh build
```
