// include/order_client_server.hpp
#ifndef ORDER_CLIENT_SERVER_HPP
#define ORDER_CLIENT_SERVER_HPP

#include "order_book.hpp"
#include "order_types.hpp"
#include "trade.hpp"
#include <memory>
#include <string>
#include <mutex>

class OrderClientServer {
public:
    explicit OrderClientServer(std::shared_ptr<OrderBook> orderBook);
    
    // Returns match result with price, quantity, and status
    MatchResult submitOrder(const std::string& order_id, 
                           const std::string& trader_id,
                           const std::string& stock_symbol, 
                           double price,
                           int quantity, 
                           bool is_buy_order);
                            
    // Returns status string
    std::string cancelOrder(const std::string& order_id, bool is_buy_order);

private:
    std::shared_ptr<OrderBook> orderBook_;
    std::mutex order_mutex_;
};

#endif // ORDER_CLIENT_SERVER_HPP
