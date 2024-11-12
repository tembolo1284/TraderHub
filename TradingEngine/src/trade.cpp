// src/trade.cpp
#include "trade.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>

Trade::Trade(std::string buyOrderId,
             std::string sellOrderId,
             double tradePrice,
             int tradeQuantity)
    : buyOrderId_(std::move(buyOrderId))
    , sellOrderId_(std::move(sellOrderId))
    , tradePrice_(tradePrice)
    , tradeQuantity_(tradeQuantity)
{
}

void Trade::execute(Trader& buyer, Trader& seller) {
    double totalPrice = tradePrice_ * tradeQuantity_;
    buyer.updateBalance(-totalPrice);  // Deduct from buyer
    seller.updateBalance(totalPrice);  // Add to seller
    
    std::ostringstream oss;
    oss << "Trade executed: " 
        << tradeQuantity_ << " shares at $"
        << std::fixed << std::setprecision(2) << tradePrice_;
    std::cout << oss.str() << std::endl;
}

std::string Trade::toString() const {
    std::ostringstream oss;
    oss << "Trade{buyOrderId='" << buyOrderId_ << "', "
        << "sellOrderId='" << sellOrderId_ << "', "
        << "tradePrice=" << std::fixed << std::setprecision(2) << tradePrice_ << ", "
        << "tradeQuantity=" << tradeQuantity_ << "}";
    return oss.str();
}

