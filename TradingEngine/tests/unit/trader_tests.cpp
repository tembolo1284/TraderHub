// tests/unit/trader_tests.cpp
#include <gtest/gtest.h>
#include "trader.hpp"
#include "order.hpp"
#include "order_book.hpp"

class TraderTest : public ::testing::Test {
protected:
    // Initialize member in constructor
    TraderTest() 
        : trader()  // Initialize unique_ptr to nullptr
    {}

    void SetUp() override {
        trader = std::make_unique<Trader>("T123", "Alice", 1000.0);
    }

    std::unique_ptr<Trader> trader;
};

TEST_F(TraderTest, CreateTrader) {
    EXPECT_EQ(trader->getTraderId(), "T123");
    EXPECT_EQ(trader->getName(), "Alice");
    EXPECT_DOUBLE_EQ(trader->getBalance(), 1000.0);
}

TEST_F(TraderTest, UpdateBalance) {
    trader->updateBalance(500.0);
    EXPECT_DOUBLE_EQ(trader->getBalance(), 1500.0);
    
    trader->updateBalance(-600.0);
    EXPECT_DOUBLE_EQ(trader->getBalance(), 900.0);
}

TEST_F(TraderTest, InvalidBalanceUpdate) {
    trader->updateBalance(-500.0);
    EXPECT_DOUBLE_EQ(trader->getBalance(), 500.0);
}
