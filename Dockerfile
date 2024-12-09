# Build stage
FROM debian:bookworm-slim AS builder

# Install necessary build tools and dependencies
RUN apt-get update && \
    apt-get install -y \
        cmake \
        ninja-build \
        g++ \
        make \
        git \
        autoconf \
        libtool \
        pkg-config \
        libspdlog-dev \
        nlohmann-json3-dev \
        libgtest-dev && \
    rm -rf /var/lib/apt/lists/*

# Install gRPC from source
WORKDIR /usr/src
RUN git clone --recurse-submodules -b v1.42.0 https://github.com/grpc/grpc && \
    cd grpc && \
    mkdir -p cmake/build && \
    cd cmake/build && \
    cmake ../.. -DgRPC_INSTALL=ON \
                -DgRPC_BUILD_TESTS=OFF \
                -DCMAKE_INSTALL_PREFIX=/usr/local \
                -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc) && \
    make install && \
    ldconfig && \
    cd /usr/src && \
    rm -rf grpc

# Set the working directory for our project
WORKDIR /usr/src/TraderHub

# Copy all source files
COPY . .

# Build the project using CMake and Ninja
RUN cmake -B build -G Ninja && \
    cmake --build build

# Runtime stage
FROM debian:bookworm-slim

# Install necessary runtime libraries
RUN apt-get update && \
    apt-get install -y \
        libstdc++6 \
        libspdlog1.10 && \
    rm -rf /var/lib/apt/lists/*

# Copy gRPC and Protobuf runtime libraries from builder
COPY --from=builder /usr/local/lib/libgrpc*.so* /usr/local/lib/
COPY --from=builder /usr/local/lib/libprotobuf*.so* /usr/local/lib/
COPY --from=builder /usr/local/lib/libabsl*.so* /usr/local/lib/
COPY --from=builder /usr/local/lib/libupb*.so* /usr/local/lib/
COPY --from=builder /usr/local/lib/libaddress_sorting.so* /usr/local/lib/
COPY --from=builder /usr/local/lib/libre2.so* /usr/local/lib/

# Update library cache
RUN ldconfig

# Copy the built binaries from the build stage
COPY --from=builder /usr/src/TraderHub/build/OrderClientServer/OrderServer /usr/local/bin/OrderServer
COPY --from=builder /usr/src/TraderHub/build/OrderClientServer/OrderClient /usr/local/bin/OrderClient

# Expose the server port (adjust port number as needed)
EXPOSE 50051

# Default to running the server
ENTRYPOINT ["/usr/local/bin/OrderServer"]
