#!/bin/bash

# Function to stop and remove existing containers
cleanup() {
    echo "Cleaning up existing containers..."
    docker compose down
    docker network rm trading-net 2>/dev/null || true
    docker system prune -f
}

# Function to ensure network exists
ensure_network() {
    if ! docker network inspect trading-net >/dev/null 2>&1; then
        echo "Creating trading-net network..."
        docker network create trading-net
    fi
}

# Function to build the Docker image
build_image() {
    echo "Building Docker image..."
    ensure_network
    docker compose build
}

# Function to run the server
run_server() {
    echo "Starting server..."
    ensure_network
    docker compose up server
}

# Function to run the client
run_client() {
    echo "Starting client..."
    ensure_network
    
    # Make sure server is running first
    if ! docker ps | grep -q traderhub-server; then
        echo "Server is not running. Please start the server first with './docker-run.sh server'"
        return 1
    fi
    
    # Wait a moment for the server to be ready
    sleep 2
    
    # Run the client with all passed arguments
    if [ $# -eq 0 ]; then
        docker compose run --rm client view
    else
        docker compose run --rm client "$@"
    fi
}

# Main script
case "$1" in
    "build")
        build_image
        ;;
    "server")
        cleanup
        run_server
        ;;
    "client")
        shift
        run_client "$@"
        ;;
    "clean")
        cleanup
        ;;
    *)
        echo "Usage: $0 {build|server|client|clean} [client args]"
        echo "  build  - Build the Docker image"
        echo "  server - Run the server"
        echo "  client - Run the client with optional arguments"
        echo "  clean  - Clean up containers and networks"
        echo ""
        echo "Examples:"
        echo "  $0 client submit order1 trader1 AAPL 150.50 100 buy"
        echo "  $0 client cancel order1 buy"
        echo "  $0 client view"
        echo "  $0 client view AAPL"
        echo "  $0 client file orders.json"
        exit 1
        ;;
esac
