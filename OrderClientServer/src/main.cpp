// src/main.cpp
#include <string>
#include <memory>
#include <grpcpp/grpcpp.h>
#include "order_service.hpp"
#include "order_client_server.hpp"
#include <spdlog/spdlog.h>
// Instead of specific sink headers, we'll just use the basic functionality

class TradingServer {
public:
    TradingServer() {
        try {
            spdlog::info("Initializing TradingServer...");
            
            // Initialize components
            order_client_server_ = std::make_shared<OrderClientServer>();
            if (!order_client_server_) {
                throw std::runtime_error("Failed to create OrderClientServer");
            }
            spdlog::info("OrderClientServer created successfully");

            order_service_ = std::make_unique<OrderServiceImpl>(order_client_server_);
            if (!order_service_) {
                throw std::runtime_error("Failed to create OrderServiceImpl");
            }
            spdlog::info("OrderServiceImpl created successfully");
        }
        catch (const std::exception& e) {
            spdlog::critical("Failed to initialize TradingServer: {}", e.what());
            throw;
        }
    }

    void Run() {
        try {
            std::string server_address("0.0.0.0:50051");
            grpc::ServerBuilder builder;
            
            // Configure server
            builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
            builder.RegisterService(order_service_.get());
            
            // Set server options
            builder.SetMaxMessageSize(100 * 1024 * 1024);  // 100MB
            builder.SetMaxReceiveMessageSize(100 * 1024 * 1024);  // 100MB
            
            // Build and start the server
            server_ = builder.BuildAndStart();
            if (!server_) {
                throw std::runtime_error("Failed to start server");
            }
            
            spdlog::info("Server listening on {}", server_address);
            server_->Wait();
        }
        catch (const std::exception& e) {
            spdlog::critical("Server error: {}", e.what());
            throw;
        }
    }

    ~TradingServer() {
        try {
            if (server_) {
                spdlog::info("Shutting down server...");
                server_->Shutdown();
                spdlog::info("Server shutdown complete");
            }
        }
        catch (const std::exception& e) {
            spdlog::error("Error during server shutdown: {}", e.what());
        }
    }

private:
    std::shared_ptr<OrderClientServer> order_client_server_;
    std::unique_ptr<OrderServiceImpl> order_service_;
    std::unique_ptr<grpc::Server> server_;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    try {
        // Initialize logging with more detailed pattern
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");
        spdlog::set_level(spdlog::level::debug);
        
        spdlog::info("Starting trading server...");
        
        TradingServer server;
        server.Run();
        
        return 0;
    }
    catch (const std::exception& e) {
        spdlog::critical("Fatal error: {}", e.what());
        return 1;
    }
}
