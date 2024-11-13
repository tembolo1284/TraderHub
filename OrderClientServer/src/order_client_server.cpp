// src/order_client_server.cpp
#include "order_client_server.hpp"
#include "order_book.hpp"
#include <spdlog/spdlog.h>
#include <algorithm>

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
        // First add the order
        orderBook_->addOrder(order);
        
        spdlog::info("Added {} order to book: ID={}, Symbol={}, Price={}, Qty={}", 
                    is_buy_order ? "BUY" : "SELL", order_id, stock_symbol, price, quantity);

        // No matches yet, so return unmatched status
        MatchResult result{
            0.0,  // No match price yet
            0,    // No match quantity yet
            "Order added to book"
        };

        // Try to match orders
        orderBook_->matchOrders();

        // Check if order was matched by seeing if it still exists with remaining quantity
        if (orderBook_->isOrderCanceled(order_id, is_buy_order) || 
            !orderBook_->getQuantityAtPrice(price, is_buy_order)) {
            // Order was fully matched
            result.price = is_buy_order ? price : price;  // For buy orders, we match at sell price
            result.quantity = quantity;
            result.status = "Order fully matched";
            spdlog::info("Order {} was fully matched", order_id);
        }
        
        return result;
    }
    catch (const std::exception& e) {
        spdlog::error("Error submitting order {}: {}", order_id, e.what());
        throw OrderError("Failed to submit order: " + std::string(e.what()));
    }
}

std::string OrderClientServer::cancelOrder(const std::string& order_id, bool is_buy_order) {
    try {
        std::lock_guard<std::mutex> lock(order_mutex_);
        if (!orderBook_->isOrderCanceled(order_id, is_buy_order)) {
            orderBook_->cancelOrder(order_id, is_buy_order);
            spdlog::info("Cancelled order: ID={}, Side={}", 
                        order_id, is_buy_order ? "BUY" : "SELL");
            return "Order cancelled successfully";
        }
        return "Order not found or already cancelled";
    }
    catch (const std::exception& e) {
        spdlog::error("Error cancelling order {}: {}", order_id, e.what());
        throw OrderError("Failed to cancel order: " + std::string(e.what()));
    }
}

std::pair<std::vector<Order>, std::vector<Order>> OrderClientServer::getOrderBook(const std::string& symbol) const {
    try {
        std::lock_guard<std::mutex> lock(order_mutex_);
        
        auto buy_refs = orderBook_->getBuyOrders();
        auto sell_refs = orderBook_->getSellOrders();
        
        std::vector<std::reference_wrapper<const Order>> filtered_buys;
        std::vector<std::reference_wrapper<const Order>> filtered_sells;

        // Filter orders
        for (const auto& order_ref : buy_refs) {
            const Order& order = order_ref.get();
            if (!order.isCanceled() && order.getRemainingQuantity() > 0 &&
                (symbol.empty() || order.getStockSymbol() == symbol)) {
                filtered_buys.push_back(std::cref(order));
            }
        }

        for (const auto& order_ref : sell_refs) {
            const Order& order = order_ref.get();
            if (!order.isCanceled() && order.getRemainingQuantity() > 0 &&
                (symbol.empty() || order.getStockSymbol() == symbol)) {
                filtered_sells.push_back(std::cref(order));
            }
        }

        // Sort using stable_sort to maintain order stability
        std::stable_sort(filtered_buys.begin(), filtered_buys.end(),
            [](const auto& a, const auto& b) {
                return a.get().getPrice() > b.get().getPrice();  // Highest buy first
            });

        std::stable_sort(filtered_sells.begin(), filtered_sells.end(),
            [](const auto& a, const auto& b) {
                return a.get().getPrice() < b.get().getPrice();  // Lowest sell first
            });

        // Create vectors of Orders
        std::vector<Order> buy_orders;
        std::vector<Order> sell_orders;

        buy_orders.reserve(filtered_buys.size());
        sell_orders.reserve(filtered_sells.size());

        // Copy the sorted orders
        for (const auto& ref : filtered_buys) {
            buy_orders.push_back(ref.get());
        }

        for (const auto& ref : filtered_sells) {
            sell_orders.push_back(ref.get());
        }

        return {std::move(buy_orders), std::move(sell_orders)};
    }
    catch (const std::exception& e) {
        spdlog::error("Error getting order book: {}", e.what());
        throw OrderError("Failed to get order book: " + std::string(e.what()));
    }
}
