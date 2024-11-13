// include/order_client_server.hpp
#ifndef ORDER_CLIENT_SERVER_HPP
#define ORDER_CLIENT_SERVER_HPP

#include "order_service.grpc.pb.h"
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
    OrderClientServer() = default;
    
    order_service::OrderResponse submitOrder(const order_service::OrderRequest& request);
    order_service::CancelResponse cancelOrder(const order_service::CancelRequest& request);
    order_service::ViewOrderBookResponse getOrderBook(const order_service::ViewOrderBookRequest& request);

private:
    mutable std::mutex order_mutex_;
    std::vector<order_service::OrderBookEntry> buy_orders_;
    std::vector<order_service::OrderBookEntry> sell_orders_;
    
    // Helper methods
    int matchOrders(order_service::OrderBookEntry& new_order);
    std::string getCurrentTimestamp() const;
};

#endif // ORDER_CLIENT_SERVER_HPP
