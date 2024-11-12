// trader.hpp
#ifndef TRADER_HPP
#define TRADER_HPP

#include <string>

// Forward declarations
class Order;
class OrderBook;

class Trader {
public:
    // Constructor
    Trader(std::string traderId, std::string name, double balance);

    // Getters
    [[nodiscard]] const std::string& getTraderId() const noexcept { return traderId_; }
    [[nodiscard]] const std::string& getName() const noexcept { return name_; }
    [[nodiscard]] double getBalance() const noexcept { return balance_; }

    // Setters
    void setTraderId(std::string traderId) { traderId_ = std::move(traderId); }
    void setName(std::string name) { name_ = std::move(name); }
    void setBalance(double balance) noexcept { balance_ = balance; }

    // Trading operations
    void placeOrder(OrderBook& orderBook, const Order& order);
    void updateBalance(double amount) noexcept;

private:
    std::string traderId_;
    std::string name_;
    double balance_;
};

#endif
