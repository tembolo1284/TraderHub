// trader.cpp
#include "trader.hpp"
#include "order.hpp"
#include "order_book.hpp"

Trader::Trader(std::string traderId, std::string name, double balance)
    : traderId_(std::move(traderId))
    , name_(std::move(name))
    , balance_(balance)
{}

void Trader::placeOrder(OrderBook& orderBook, const Order& order) {
    orderBook.addOrder(const_cast<Order&>(order));  // Safe as we know the order exists
}

void Trader::updateBalance(double amount) noexcept {
    balance_ += amount;
}
