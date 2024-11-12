// order.cpp
#include "order.hpp"
#include <sstream>
#include <iostream>

Order::Order(std::string orderId,
             std::string traderId,
             std::string stockSymbol,
             double price,
             int quantity,
             bool isBuyOrder)
    : orderId_(std::move(orderId))
    , traderId_(std::move(traderId))
    , stockSymbol_(std::move(stockSymbol))
    , price_(price)
    , isBuyOrder_(isBuyOrder)
    , timestamp_(std::chrono::system_clock::now().time_since_epoch().count())
    , quantity_(quantity)
    , remainingQuantity_(quantity)
    , isCanceled_(false)
{
    if (quantity <= 0) {
        throw std::invalid_argument("Order quantity must be positive");
    }
}

void Order::setPrice(double newPrice) noexcept {
    price_ = newPrice;
}

void Order::setQuantity(int newQuantity) {
    if (newQuantity <= 0) {
        throw std::invalid_argument("New quantity must be positive");
    }
    quantity_ = newQuantity;
    remainingQuantity_ = newQuantity;
}

void Order::reduceQuantity(int amount) {
    if (amount <= 0) {
        throw std::invalid_argument("Amount to reduce must be positive");
    }
    if (amount > remainingQuantity_) {
        std::cout << "Error in reduceQuantity: amount = " << amount 
                  << ", remainingQuantity = " << remainingQuantity_ << std::endl;
        throw std::invalid_argument("Amount to reduce exceeds remaining quantity");
    }
    std::cout << "Reducing quantity by " << amount 
              << " for Order ID " << orderId_ << " with remaining quantity = " 
              << remainingQuantity_ << std::endl;
    remainingQuantity_ -= amount;
    std::cout << "New remaining quantity for Order ID " << orderId_ 
              << " = " << remainingQuantity_ << std::endl;
}


void Order::cancel() noexcept {
    isCanceled_ = true;
    // remainingQuantity_ = 0;
}

bool Order::operator==(const Order& other) const noexcept {
    return orderId_ == other.orderId_ &&
           traderId_ == other.traderId_ &&
           stockSymbol_ == other.stockSymbol_ &&
           price_ == other.price_ &&
           quantity_ == other.quantity_ &&
           remainingQuantity_ == other.remainingQuantity_ &&
           isBuyOrder_ == other.isBuyOrder_;
}

std::strong_ordering Order::operator<=>(const Order& other) const noexcept {
    // First compare by order type (buy/sell)
    if (isBuyOrder_ != other.isBuyOrder_) {
        return isBuyOrder_ ? std::strong_ordering::less : std::strong_ordering::greater;
    }

    // For buy orders, higher price has priority (comes first)
    // For sell orders, lower price has priority (comes first)
    if (price_ != other.price_) {
        if (isBuyOrder_) {
            return price_ > other.price_ ? std::strong_ordering::less : std::strong_ordering::greater;
        } else {
            return price_ < other.price_ ? std::strong_ordering::less : std::strong_ordering::greater;
        }
    }

    // If prices are equal, earlier timestamp has priority (comes first)
    return timestamp_ <=> other.timestamp_;
}

std::string Order::toString() const {
    std::ostringstream oss;
    oss << "Order{"
        << "orderId='" << orderId_ << "'"
        << ", traderId='" << traderId_ << "'"
        << ", stockSymbol='" << stockSymbol_ << "'"
        << ", price=" << price_
        << ", quantity=" << quantity_
        << ", remainingQuantity=" << remainingQuantity_
        << ", isBuyOrder=" << isBuyOrder_
        << ", timestamp=" << timestamp_
        << ", isCanceled=" << isCanceled_
        << "}";
    return oss.str();
}
