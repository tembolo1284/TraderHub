// tests/unit/trade_tests.cpp
#include <gtest/gtest.h>
#include "trade.hpp"
#include "trader.hpp"

class TradeTest : public ::testing::Test {
protected:
    // Initialize all members in constructor
    TradeTest()
        : trade()
        , buyer()
        , seller()
    {}

    void SetUp() override {
        trade = std::make_unique<Trade>("O1", "O2", 150.0, 10);
        buyer = std::make_unique<Trader>("T1", "Alice", 2000.0);
        seller = std::make_unique<Trader>("T2", "Bob", 500.0);
    }

    std::unique_ptr<Trade> trade;
    std::unique_ptr<Trader> buyer;
    std::unique_ptr<Trader> seller;
};

TEST_F(TradeTest, CreateTrade) {
    EXPECT_EQ(trade->getBuyOrderId(), "O1");
    EXPECT_EQ(trade->getSellOrderId(), "O2");
    EXPECT_DOUBLE_EQ(trade->getTradePrice(), 150.0);
    EXPECT_EQ(trade->getTradeQuantity(), 10);
}

TEST_F(TradeTest, ExecuteTrade) {
    trade->execute(*buyer, *seller);

    double expectedBuyerBalance = 2000.0 - (150.0 * 10);
    double expectedSellerBalance = 500.0 + (150.0 * 10);

    EXPECT_DOUBLE_EQ(buyer->getBalance(), expectedBuyerBalance);
    EXPECT_DOUBLE_EQ(seller->getBalance(), expectedSellerBalance);
}

TEST_F(TradeTest, ExecuteTradeWithInsufficientBalance) {
    std::unique_ptr<Trader> poorBuyer = 
        std::make_unique<Trader>("T3", "Charlie", 100.0);
    
    EXPECT_NO_THROW(trade->execute(*poorBuyer, *seller));
    EXPECT_DOUBLE_EQ(poorBuyer->getBalance(), -1400.0); // Allow negative balance
}
