// src/order_client_server.cpp
#include "order_client_server.hpp"
#include <spdlog/spdlog.h>

OrderClientServer::OrderClientServer(std::shared_ptr<OrderBook> orderBook)
    : orderBook_(std::move(orderBook)) {
    if (!orderBook_) {
        throw std::invalid_argument("OrderBook cannot be null");
    }
}

MatchResult OrderClientServer::submitOrder(const std::string& order_id, 
                                         const std::string& trader_id,
                                         const std::string& stock_symbol, 
                                         double price,
                                         int quantity, 
                                         bool is_buy_order) {
    try {
        Order order(order_id, trader_id, stock_symbol, price, quantity, is_buy_order);
        
        std::lock_guard<std::mutex> lock(order_mutex_);
        orderBook_->addOrder(order);
        
        // Assuming matchOrders() returns void, we'll just return a basic result
        orderBook_->matchOrders();
        
        // Since we don't have direct access to match results,
        // we'll return a basic confirmation
        return MatchResult{
            price,  // Using submitted price
            quantity,  // Using submitted quantity
            "Order processed successfully"
        };
    }
    catch (const std::exception& e) {
        spdlog::error("Error submitting order {}: {}", order_id, e.what());
        throw OrderError("Failed to submit order: " + std::string(e.what()));
    }
}

std::string OrderClientServer::cancelOrder(const std::string& order_id, bool is_buy_order) {
    try {
        std::lock_guard<std::mutex> lock(order_mutex_);
        orderBook_->cancelOrder(order_id, is_buy_order);  // Assuming void return
        return "Order cancelled successfully";
    }
    catch (const std::exception& e) {
        spdlog::error("Error cancelling order {}: {}", order_id, e.what());
        throw OrderError("Failed to cancel order: " + std::string(e.what()));
    }
}
