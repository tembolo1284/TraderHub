// src/order_service.cpp
#include "order_service.hpp"
#include <spdlog/spdlog.h>
#include <chrono>
#include <iomanip>
#include <thread>

namespace {
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
}

OrderServiceImpl::OrderServiceImpl(std::shared_ptr<OrderClientServer> server)
    : server_(std::move(server)) {
    if (!server_) {
        throw std::invalid_argument("Server cannot be null");
    }
}

grpc::Status OrderServiceImpl::SubmitOrder(grpc::ServerContext* context,
                                         const order_service::OrderRequest* request,
                                         order_service::OrderResponse* response) {
    try {
        spdlog::info("Received order: ID={}, Symbol={}, Price={}, Qty={}, Side={}", 
            request->details().order_id(),
            request->details().stock_symbol(),
            request->details().price(),
            request->details().quantity(),
            request->details().is_buy_order() ? "BUY" : "SELL");
        
        *response = server_->submitOrder(*request);
        return grpc::Status::OK;
    }
    catch (const std::exception& e) {
        spdlog::error("Failed to submit order: {}", e.what());
        return grpc::Status(grpc::StatusCode::INTERNAL, 
                          std::string("Failed to submit order: ") + e.what());
    }
}

grpc::Status OrderServiceImpl::CancelOrder(grpc::ServerContext* context,
                                         const order_service::CancelRequest* request,
                                         order_service::CancelResponse* response) {
    try {
        spdlog::info("Received cancel request: Order ID={}, Side={}", 
            request->order_id(),
            request->is_buy_order() ? "BUY" : "SELL");
        
        *response = server_->cancelOrder(*request);
        response->set_timestamp(getCurrentTimestamp());
        return grpc::Status::OK;
    }
    catch (const std::exception& e) {
        spdlog::error("Failed to cancel order: {}", e.what());
        return grpc::Status(grpc::StatusCode::INTERNAL, 
                          std::string("Failed to cancel order: ") + e.what());
    }
}

grpc::Status OrderServiceImpl::ViewOrderBook(grpc::ServerContext* context,
                                           const order_service::ViewOrderBookRequest* request,
                                           order_service::ViewOrderBookResponse* response) {
    try {
        spdlog::info("Received order book request{}", 
            request->symbol().empty() ? "" : " for symbol " + request->symbol());
        
        *response = server_->getOrderBook(*request);
        response->set_timestamp(getCurrentTimestamp());
        
        spdlog::info("Returning order book with {} buy orders and {} sell orders",
                    response->buy_orders_size(),
                    response->sell_orders_size());
        
        return grpc::Status::OK;
    }
    catch (const std::exception& e) {
        spdlog::error("Failed to get order book: {}", e.what());
        return grpc::Status(grpc::StatusCode::INTERNAL, 
                          std::string("Failed to get order book: ") + e.what());
    }
}

grpc::Status OrderServiceImpl::StreamOrderBook(grpc::ServerContext* context,
                                             const order_service::ViewOrderBookRequest* request,
                                             grpc::ServerWriter<order_service::ViewOrderBookResponse>* writer) {
    try {
        spdlog::info("Starting order book stream{}", 
            request->symbol().empty() ? "" : " for symbol " + request->symbol());
            
        // Implementation for streaming updates
        // This is a basic implementation that just sends periodic updates
        while (!context->IsCancelled()) {
            order_service::ViewOrderBookResponse response = server_->getOrderBook(*request);
            response.set_timestamp(getCurrentTimestamp());
            
            if (!writer->Write(response)) {
                spdlog::warn("Failed to write to stream, client may have disconnected");
                break;
            }
            
            // Sleep for a short duration before sending the next update
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        spdlog::info("Order book stream ended");
        return grpc::Status::OK;
    }
    catch (const std::exception& e) {
        spdlog::error("Failed to stream order book: {}", e.what());
        return grpc::Status(grpc::StatusCode::INTERNAL, 
                          std::string("Failed to stream order book: ") + e.what());
    }
}
