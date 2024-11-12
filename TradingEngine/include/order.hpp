// order.hpp
#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>
#include <chrono>
#include <compare>
#include <stdexcept>

class Order {
public:
    // Constructor
    Order(std::string orderId,
          std::string traderId,
          std::string stockSymbol,
          double price,
          int quantity,
          bool isBuyOrder);

    // Getters
    [[nodiscard]] const std::string& getOrderId() const noexcept { return orderId_; }
    [[nodiscard]] const std::string& getTraderId() const noexcept { return traderId_; }
    [[nodiscard]] const std::string& getStockSymbol() const noexcept { return stockSymbol_; }
    [[nodiscard]] double getPrice() const noexcept { return price_; }
    [[nodiscard]] int getQuantity() const noexcept { return quantity_; }
    [[nodiscard]] int getRemainingQuantity() const noexcept { return remainingQuantity_; }
    [[nodiscard]] bool isBuyOrder() const noexcept { return isBuyOrder_; }
    [[nodiscard]] int64_t getTimestamp() const noexcept { return timestamp_; }
    [[nodiscard]] bool isCanceled() const noexcept { return isCanceled_; }

    // Setters
    void setPrice(double newPrice) noexcept;
    void setQuantity(int newQuantity);
    void reduceQuantity(int amount);
    void cancel() noexcept;

    // Comparison operators
    [[nodiscard]] bool operator==(const Order& other) const noexcept;
    [[nodiscard]] std::strong_ordering operator<=>(const Order& other) const noexcept;

    // String representation
    [[nodiscard]] std::string toString() const;

private:
    const std::string orderId_;
    const std::string traderId_;
    const std::string stockSymbol_;
    double price_;
    const bool isBuyOrder_;
    const int64_t timestamp_;
    int quantity_;
    int remainingQuantity_;
    bool isCanceled_;
};

#endif
