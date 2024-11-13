// include/order_book.hpp
#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include "order.hpp"
#include "prioritizable_value_st.hpp"
#include <memory>
#include <vector>
#include <functional>

class OrderBook {
public:
    OrderBook();
    ~OrderBook();

    void addOrder(Order& order);
    void matchOrders();
    void cancelOrder(const std::string& orderId, bool isBuyOrder);
    int getQuantityAtPrice(double price, bool isBuyOrder) const;
    bool isOrderCanceled(const std::string& orderId, bool isBuyOrder) const;

    // Updated return types to match PrioritizableValueST
    std::vector<std::reference_wrapper<Order>> getBuyOrders() const {
        return buyOrders->getAllValues();
    }

    std::vector<std::reference_wrapper<Order>> getSellOrders() const {
        return sellOrders->getAllValues();
    }

private:
    std::unique_ptr<PrioritizableValueST<std::string, Order>> buyOrders;
    std::unique_ptr<PrioritizableValueST<std::string, Order>> sellOrders;

    bool isMatchPossible(const Order& buyOrder, const Order& sellOrder) const;
    void processMatch(Order& buyOrder, Order& sellOrder);
    void reinsertOrder(Order& order, bool isBuyOrder);
};

#endif // ORDER_BOOK_HPP
