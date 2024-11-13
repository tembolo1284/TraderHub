// src/order_client_server.cpp
#include "order_client_server.hpp"
#include <spdlog/spdlog.h>
#include <algorithm>
#include <chrono>
#include <iomanip>

std::string OrderClientServer::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

order_service::OrderResponse OrderClientServer::submitOrder(const order_service::OrderRequest& request) {
    try {
        std::lock_guard<std::mutex> lock(order_mutex_);
        order_service::OrderResponse response;
        
        spdlog::info("Processing order: ID={}, Symbol={}, Price={}, Qty={}, Side={}", 
                     request.details().order_id(),
                     request.details().stock_symbol(),
                     request.details().price(),
                     request.details().quantity(),
                     request.details().is_buy_order() ? "BUY" : "SELL");

        // Create initial order book entry
        order_service::OrderBookEntry new_order;
        *new_order.mutable_details() = request.details();
        new_order.set_remaining_quantity(request.details().quantity());
        new_order.set_timestamp(getCurrentTimestamp());

        // Try to match the order
        int matched_quantity = matchOrders(new_order);
        int remaining_quantity = request.details().quantity() - matched_quantity;

        // Set response based on matching results
        if (matched_quantity == request.details().quantity()) {
            response.set_status(order_service::OrderStatus::FULLY_FILLED);
            response.set_message("Order fully matched");
        } else if (matched_quantity > 0) {
            response.set_status(order_service::OrderStatus::PARTIAL_FILL);
            response.set_message("Order partially matched");
            
            // Add remaining quantity to book
            new_order.set_remaining_quantity(remaining_quantity);
            if (request.details().is_buy_order()) {
                buy_orders_.push_back(new_order);
            } else {
                sell_orders_.push_back(new_order);
            }
        } else {
            response.set_status(order_service::OrderStatus::SUCCESS);
            response.set_message("Order added to book");
            
            // No matches, add to book
            if (request.details().is_buy_order()) {
                buy_orders_.push_back(new_order);
            } else {
                sell_orders_.push_back(new_order);
            }
        }

        response.set_matched_price(request.details().price());
        response.set_matched_quantity(matched_quantity);
        response.set_timestamp(getCurrentTimestamp());
        
        return response;
    }
    catch (const std::exception& e) {
        spdlog::error("Error processing order {}: {}", 
                      request.details().order_id(), e.what());
        throw OrderError("Failed to process order: " + std::string(e.what()));
    }
}

int OrderClientServer::matchOrders(order_service::OrderBookEntry& new_order) {
    auto& opposite_orders = new_order.details().is_buy_order() ? sell_orders_ : buy_orders_;
    int total_matched = 0;
    
    // Sort opposite orders by price
    std::sort(opposite_orders.begin(), opposite_orders.end(),
        [&new_order](const auto& a, const auto& b) {
            if (new_order.details().is_buy_order()) {
                return a.details().price() < b.details().price(); // For buy orders, lowest sell first
            } else {
                return a.details().price() > b.details().price(); // For sell orders, highest buy first
            }
        });
    
    // Try to match orders
    auto it = opposite_orders.begin();
    int remaining_to_match = new_order.remaining_quantity(); // Track remaining quantity

    while (it != opposite_orders.end() && remaining_to_match > 0) {
        // Check if prices cross
        if ((new_order.details().is_buy_order() && 
             new_order.details().price() >= it->details().price()) ||
            (!new_order.details().is_buy_order() && 
             new_order.details().price() <= it->details().price())) {
            
            int match_quantity = std::min(remaining_to_match, 
                                        it->remaining_quantity());
            
            spdlog::info("Match found: Order {} matches with {} for quantity {}", 
                        new_order.details().order_id(), 
                        it->details().order_id(),
                        match_quantity);
            
            total_matched += match_quantity;
            remaining_to_match -= match_quantity;
            new_order.set_remaining_quantity(remaining_to_match); // Update remaining quantity
            it->set_remaining_quantity(it->remaining_quantity() - match_quantity);
            
            if (it->remaining_quantity() == 0) {
                it = opposite_orders.erase(it);
            } else {
                ++it;
            }
        } else {
            // No more possible matches due to price
            break;
        }
    }
    
    return total_matched;
}

order_service::CancelResponse OrderClientServer::cancelOrder(const order_service::CancelRequest& request) {
    try {
        std::lock_guard<std::mutex> lock(order_mutex_);
        order_service::CancelResponse response;
        
        auto& orders = request.is_buy_order() ? buy_orders_ : sell_orders_;
        auto it = std::find_if(orders.begin(), orders.end(),
            [&request](const auto& entry) {
                return entry.details().order_id() == request.order_id();
            });
            
        if (it != orders.end()) {
            orders.erase(it);
            response.set_status(order_service::OrderStatus::CANCELLED);
            response.set_message("Order cancelled successfully");
        } else {
            response.set_status(order_service::OrderStatus::ERROR);
            response.set_message("Order not found");
        }
        
        response.set_timestamp(getCurrentTimestamp());
        return response;
    }
    catch (const std::exception& e) {
        spdlog::error("Failed to cancel order: {}", e.what());
        throw OrderError("Failed to cancel order: " + std::string(e.what()));
    }
}

order_service::ViewOrderBookResponse OrderClientServer::getOrderBook(
    const order_service::ViewOrderBookRequest& request) {
    try {
        std::lock_guard<std::mutex> lock(order_mutex_);
        order_service::ViewOrderBookResponse response;
        
        // Copy relevant orders to response
        for (const auto& order : buy_orders_) {
            if (request.symbol().empty() || 
                order.details().stock_symbol() == request.symbol()) {
                *response.add_buy_orders() = order;
            }
        }
        
        for (const auto& order : sell_orders_) {
            if (request.symbol().empty() || 
                order.details().stock_symbol() == request.symbol()) {
                *response.add_sell_orders() = order;
            }
        }
        
        response.set_total_buy_orders(response.buy_orders_size());
        response.set_total_sell_orders(response.sell_orders_size());
        response.set_timestamp(getCurrentTimestamp());
        response.set_symbol(request.symbol());
        
        return response;
    }
    catch (const std::exception& e) {
        spdlog::error("Failed to get order book: {}", e.what());
        throw OrderError("Failed to get order book: " + std::string(e.what()));
    }
}
