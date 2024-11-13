// include/order_client_server.hpp
#ifndef ORDER_CLIENT_SERVER_HPP
#define ORDER_CLIENT_SERVER_HPP

#include "order_book.hpp"
#include "order_types.hpp"
#include "trade.hpp"
#include <memory>
#include <string>
#include <mutex>
#include <utility>
#include <vector>

class OrderError : public std::runtime_error {
public:
    explicit OrderError(const std::string& message) : std::runtime_error(message) {}
};

class OrderClientServer {
public:
    explicit OrderClientServer(std::shared_ptr<OrderBook> orderBook);
    
    MatchResult submitOrder(const std::string& order_id, 
                           const std::string& trader_id,
                           const std::string& stock_symbol, 
                           double price,
                           int quantity, 
                           bool is_buy_order);
                            
    std::string cancelOrder(const std::string& order_id, bool is_buy_order);

    // New method
    std::pair<std::vector<Order>, std::vector<Order>> getOrderBook(const std::string& symbol = "") const;

private:
    std::shared_ptr<OrderBook> orderBook_;
    mutable std::mutex order_mutex_;
};

#endif // ORDER_CLIENT_SERVER_HPP
