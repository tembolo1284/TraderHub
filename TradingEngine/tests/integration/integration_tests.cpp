#include <gtest/gtest.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include "order_book.hpp"
#include "trader.hpp"
#include "trade.hpp"
#include "order.hpp"

class TradingSystemIntegrationTest : public ::testing::Test {
protected:
    // Initialize all members in constructor
    TradingSystemIntegrationTest()
        : orderBook()
        , buyer1()
        , buyer2()
        , seller1()
        , seller2()
    {}

    void SetUp() override {
        orderBook = std::make_unique<OrderBook>();
        
        // Create traders with initial balances
        buyer1 = std::make_unique<Trader>("T1", "Alice", 10000.0);
        buyer2 = std::make_unique<Trader>("T2", "Bob", 15000.0);
        seller1 = std::make_unique<Trader>("T3", "Carol", 5000.0);
        seller2 = std::make_unique<Trader>("T4", "Dave", 7500.0);
    }

    std::unique_ptr<OrderBook> orderBook;
    std::unique_ptr<Trader> buyer1;
    std::unique_ptr<Trader> buyer2;
    std::unique_ptr<Trader> seller1;
    std::unique_ptr<Trader> seller2;
};

TEST_F(TradingSystemIntegrationTest, MultipleTradesWithPriceTimePriority) {
    // Add multiple buy orders with different prices and timestamps
    auto buyOrder1 = Order("B1", buyer1->getTraderId(), "AAPL", 150.0, 100, true);
    auto buyOrder2 = Order("B2", buyer2->getTraderId(), "AAPL", 151.0, 50, true);
    
    // Add sell orders with different prices
    auto sellOrder1 = Order("S1", seller1->getTraderId(), "AAPL", 149.0, 75, false);
    auto sellOrder2 = Order("S2", seller2->getTraderId(), "AAPL", 150.0, 50, false);

    // Place orders in sequence with small delays
    orderBook->addOrder(buyOrder1);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    orderBook->addOrder(buyOrder2);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    orderBook->addOrder(sellOrder1);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    orderBook->addOrder(sellOrder2);

    // Match orders
    orderBook->matchOrders();

    // Verify correct matching based on price-time priority
    EXPECT_EQ(buyOrder1.getRemainingQuantity(), 25);  // Should have 25 remaining after matching
    EXPECT_EQ(buyOrder2.getRemainingQuantity(), 0);   // Should be fully matched
    EXPECT_EQ(sellOrder1.getRemainingQuantity(), 0);  // Should be fully matched
    EXPECT_EQ(sellOrder2.getRemainingQuantity(), 0);  // Should be fully matched
}

TEST_F(TradingSystemIntegrationTest, OrderCancellationDuringTrading) {
    // Set up initial orders
    auto buyOrder = Order("B1", buyer1->getTraderId(), "AAPL", 150.0, 100, true);
    auto sellOrder1 = Order("S1", seller1->getTraderId(), "AAPL", 149.0, 50, false);
    auto sellOrder2 = Order("S2", seller2->getTraderId(), "AAPL", 149.0, 50, false);

    orderBook->addOrder(buyOrder);
    orderBook->addOrder(sellOrder1);
    orderBook->addOrder(sellOrder2);
    
    // Cancel second sell order before matching
    orderBook->cancelOrder("S2", false);
    
    // Add the sell order and verify it's cancelled
    EXPECT_TRUE(sellOrder2.isCanceled());

    // Match orders
    orderBook->matchOrders();

    // Verify only non-cancelled orders were matched
    std::cout << "buyOrder remainingQuantity after matching: " << buyOrder.getRemainingQuantity() << "\n";
    std::cout << "sellOrder1 remainingQuantity after matching: " << sellOrder1.getRemainingQuantity() << "\n";
    std::cout << "sellOrder2 remainingQuantity after matching: " << sellOrder2.getRemainingQuantity() << "\n";
    EXPECT_EQ(buyOrder.getRemainingQuantity(), 50);
    EXPECT_EQ(sellOrder1.getRemainingQuantity(), 0);
    EXPECT_EQ(sellOrder2.getRemainingQuantity(), 50);
}

TEST_F(TradingSystemIntegrationTest, PartialFillsAndReMatching) {
    // Create orders for partial filling
    auto buyOrder1 = Order("B1", "T1", "AAPL", 150.0, 100, true);
    auto buyOrder2 = Order("B2", "T2", "AAPL", 149.0, 50, true);
    auto sellOrder1 = Order("S1", "T3", "AAPL", 148.0, 75, false);
    auto sellOrder2 = Order("S2", "T4", "AAPL", 149.0, 100, false);

    // Add orders with delays to establish clear timestamps
    orderBook->addOrder(buyOrder1);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    orderBook->addOrder(buyOrder2);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    orderBook->addOrder(sellOrder1);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    orderBook->addOrder(sellOrder2);

    // First matching round
    orderBook->matchOrders();

    // Verify partial fills
    std::cout << "buyOrder1 remainingQuantity after matching: " << buyOrder1.getRemainingQuantity() << "\n";
    std::cout << "buyOrder2 remainingQuantity after matching: " << buyOrder2.getRemainingQuantity() << "\n";
    std::cout << "sellOrder1 remainingQuantity after matching: " << sellOrder1.getRemainingQuantity() << "\n";
    std::cout << "sellOrder2 remainingQuantity after matching: " << sellOrder2.getRemainingQuantity() << "\n";
    EXPECT_EQ(buyOrder1.getRemainingQuantity(), 0);
    EXPECT_EQ(buyOrder2.getRemainingQuantity(), 0);
    EXPECT_EQ(sellOrder1.getRemainingQuantity(), 0);
    EXPECT_EQ(sellOrder2.getRemainingQuantity(), 25);

    // Add new matching sell order
    auto buyOrder3 = Order("B3", "T5", "AAPL", 149.0, 25, true);
    orderBook->addOrder(buyOrder3);

    // Second matching round
    orderBook->matchOrders();

    // Verify final state
    EXPECT_EQ(buyOrder1.getRemainingQuantity(), 0);
    EXPECT_EQ(buyOrder2.getRemainingQuantity(), 0);
    EXPECT_EQ(buyOrder3.getRemainingQuantity(), 0);
    EXPECT_EQ(sellOrder1.getRemainingQuantity(), 0);
    EXPECT_EQ(sellOrder2.getRemainingQuantity(), 0);
}

TEST_F(TradingSystemIntegrationTest, StressTest) {
    constexpr int numOrders = 1000;
    std::vector<Order> orders;
    orders.reserve(static_cast<std::size_t>(numOrders));

    // Generate orders
    for (int i = 0; i < numOrders; ++i) {
        bool isBuyOrder = (i % 2 == 0);
        std::string orderId = "O" + std::to_string(i);
        std::string traderId = isBuyOrder ? buyer1->getTraderId() : seller1->getTraderId();
        
        orders.emplace_back(orderId, traderId, "AAPL", 
                          150.0 + static_cast<double>(i % 10), 
                          10, isBuyOrder);
    }

    // Add all orders
    for (auto& order : orders) {  // Changed from const auto& to auto&
        orderBook->addOrder(order);
    }

    // Perform multiple matching rounds
    for (int i = 0; i < 5; ++i) {
        orderBook->matchOrders();
    }

    // Verify that some matching occurred
    int totalRemainingQuantity = 0;
    for (const auto& order : orders) {  // This can stay const since we're just reading
        totalRemainingQuantity += order.getRemainingQuantity();
    }

    EXPECT_LT(totalRemainingQuantity, numOrders * 10);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
