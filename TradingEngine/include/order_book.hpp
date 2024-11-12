// order_book.hpp
#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include <memory>
#include <string>
#include "order.hpp"
#include "prioritizable_value_st.hpp"

class OrderBook {
public:
    // Constructor
    OrderBook();

    // Core functionality
    void addOrder(Order& order);
    void cancelOrder(const std::string& orderId, bool isBuyOrder);
    [[nodiscard]] bool isOrderCanceled(const std::string& orderId, bool isBuyOrder) const;
    [[nodiscard]] int getQuantityAtPrice(double price, bool isBuyOrder) const;
    void matchOrders();

    // Rule of 5
    OrderBook(const OrderBook&) = delete;
    OrderBook& operator=(const OrderBook&) = delete;
    OrderBook(OrderBook&&) noexcept = default;
    OrderBook& operator=(OrderBook&&) noexcept = default;
    ~OrderBook();

private:
    // Member variables
    std::unique_ptr<PrioritizableValueST<std::string, Order>> buyOrders;
    std::unique_ptr<PrioritizableValueST<std::string, Order>> sellOrders;

    // Helper methods
    void reinsertOrder(Order& order, bool isBuyOrder);
    [[nodiscard]] bool isMatchPossible(const Order& buyOrder, const Order& sellOrder) const;
    void processMatch(Order& buyOrder, Order& sellOrder);
};

#endif
