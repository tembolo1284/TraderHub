// include/trade.hpp
#ifndef TRADE_HPP
#define TRADE_HPP

#include <string>
#include "trader.hpp"

class Trade {
public:
    // Constructor
    Trade(std::string buyOrderId,
          std::string sellOrderId,
          double tradePrice,
          int tradeQuantity);

    // Getters
    [[nodiscard]] const std::string& getBuyOrderId() const noexcept { return buyOrderId_; }
    [[nodiscard]] const std::string& getSellOrderId() const noexcept { return sellOrderId_; }
    [[nodiscard]] double getTradePrice() const noexcept { return tradePrice_; }
    [[nodiscard]] int getTradeQuantity() const noexcept { return tradeQuantity_; }

    // Execution
    void execute(Trader& buyer, Trader& seller);

    // String representation
    [[nodiscard]] std::string toString() const;

private:
    std::string buyOrderId_;
    std::string sellOrderId_;
    double tradePrice_;
    int tradeQuantity_;
};

#endif
