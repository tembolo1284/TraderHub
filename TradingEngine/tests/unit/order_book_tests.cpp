#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include "order_book.hpp"
#include "order.hpp"

class OrderBookTest : public ::testing::Test {
protected:
    // Initialize member in constructor
    OrderBookTest() 
        : orderBook()  // Initialize unique_ptr to nullptr
    {}

    void SetUp() override {
        orderBook = std::make_unique<OrderBook>();
    }

    std::unique_ptr<OrderBook> orderBook;
};

TEST_F(OrderBookTest, AddOrderToOrderBook) {
    Order buyOrder("O1", "T123", "AAPL", 150.0, 10, true);
    Order sellOrder("O2", "T124", "AAPL", 145.0, 10, false);

    orderBook->addOrder(buyOrder);
    orderBook->addOrder(sellOrder);

    EXPECT_EQ(orderBook->getQuantityAtPrice(150.0, true), 10);
    EXPECT_EQ(orderBook->getQuantityAtPrice(145.0, false), 10);
}

TEST_F(OrderBookTest, MatchOrdersInOrderBook) {
    Order buyOrder("O1", "T123", "AAPL", 150.0, 10, true);
    Order sellOrder("O2", "T124", "AAPL", 145.0, 10, false);

    orderBook->addOrder(buyOrder);
    orderBook->addOrder(sellOrder);

    orderBook->matchOrders();

    EXPECT_EQ(orderBook->getQuantityAtPrice(150.0, true), 0);
    EXPECT_EQ(orderBook->getQuantityAtPrice(145.0, false), 0);
}

TEST_F(OrderBookTest, GetQuantityAtPriceBuyOrder) {
    Order buyOrder1("O1", "T123", "AAPL", 150.0, 10, true);
    Order buyOrder2("O2", "T124", "AAPL", 150.0, 5, true);

    orderBook->addOrder(buyOrder1);
    orderBook->addOrder(buyOrder2);

    int quantityAt150 = orderBook->getQuantityAtPrice(150.0, true);
    EXPECT_EQ(quantityAt150, 15);
}

TEST_F(OrderBookTest, GetQuantityAtPriceSellOrder) {
    Order sellOrder1("O1", "T126", "AAPL", 145.0, 10, false);
    Order sellOrder2("O2", "T127", "AAPL", 145.0, 5, false);

    orderBook->addOrder(sellOrder1);
    orderBook->addOrder(sellOrder2);

    int quantityAt145 = orderBook->getQuantityAtPrice(145.0, false);
    EXPECT_EQ(quantityAt145, 15);
}

TEST_F(OrderBookTest, PartialFill) {
    // Create test orders
    auto buyOrder = Order("B1", "T1", "AAPL", 150.0, 10, true);
    auto sellOrder = Order("S1", "T2", "AAPL", 150.0, 5, false);

    // Debug output initial state
    std::cout << "\nInitial state:" << std::endl
              << "Buy order: " << buyOrder.toString() << std::endl
              << "Sell order: " << sellOrder.toString() << std::endl;

    // Add orders to the book
    orderBook->addOrder(buyOrder);
    orderBook->addOrder(sellOrder);

    // Verify orders were added correctly
    EXPECT_EQ(orderBook->getQuantityAtPrice(150.0, true), 10);
    EXPECT_EQ(orderBook->getQuantityAtPrice(150.0, false), 5);

    // Match orders
    orderBook->matchOrders();

    // Get current order book state
    int remainingBuyQuantity = orderBook->getQuantityAtPrice(150.0, true);
    int remainingSellQuantity = orderBook->getQuantityAtPrice(150.0, false);

    // Verify the matching results
    EXPECT_EQ(remainingBuyQuantity, 5) 
        << "Order book should have 5 remaining buy units";
    EXPECT_EQ(remainingSellQuantity, 0) 
        << "Order book should have 0 remaining sell units";
}

// Add more partial fill test cases
TEST_F(OrderBookTest, MultiplePartialFills) {
    // Create a large buy order
    Order buyOrder("B1", "T1", "AAPL", 150.0, 20, true);
    
    // Create multiple smaller sell orders
    Order sellOrder1("S1", "T2", "AAPL", 150.0, 5, false);
    Order sellOrder2("S2", "T3", "AAPL", 150.0, 7, false);
    Order sellOrder3("S3", "T4", "AAPL", 150.0, 3, false);

    // Add all orders
    orderBook->addOrder(buyOrder);
    orderBook->addOrder(sellOrder1);
    orderBook->addOrder(sellOrder2);
    orderBook->addOrder(sellOrder3);

    // Match orders
    orderBook->matchOrders();

    // Verify results
    EXPECT_EQ(buyOrder.getRemainingQuantity(), 5) 
        << "Buy order should have 5 units remaining after multiple fills";
    EXPECT_EQ(sellOrder1.getRemainingQuantity(), 0) 
        << "First sell order should be completely filled";
    EXPECT_EQ(sellOrder2.getRemainingQuantity(), 0) 
        << "Second sell order should be completely filled";
    EXPECT_EQ(sellOrder3.getRemainingQuantity(), 0) 
        << "Third sell order should be completely filled";

    // Verify order book state
    EXPECT_EQ(orderBook->getQuantityAtPrice(150.0, true), 5) 
        << "Remaining buy quantity incorrect in order book";
    EXPECT_EQ(orderBook->getQuantityAtPrice(150.0, false), 0) 
        << "All sell orders should be matched";
}

TEST_F(OrderBookTest, PartialFillWithPricePriority) {
    // Create buy order
    Order buyOrder("B1", "T1", "AAPL", 150.0, 10, true);
    
    // Create sell orders with different prices
    Order sellOrder1("S1", "T2", "AAPL", 149.0, 4, false);  // Better price
    Order sellOrder2("S2", "T3", "AAPL", 150.0, 4, false);  // Worse price

    // Add orders in reverse price priority
    orderBook->addOrder(buyOrder);
    orderBook->addOrder(sellOrder2);  // Add worse price first
    orderBook->addOrder(sellOrder1);  // Add better price second

    // Match orders
    orderBook->matchOrders();

    // Verify price priority was respected
    EXPECT_EQ(buyOrder.getRemainingQuantity(), 2) 
        << "Buy order should have 2 units remaining";
    EXPECT_EQ(sellOrder1.getRemainingQuantity(), 0) 
        << "Better priced sell order should be completely filled";
    EXPECT_EQ(sellOrder2.getRemainingQuantity(), 0) 
        << "Worse priced sell order should be completely filled";

    // Verify order book state
    EXPECT_EQ(orderBook->getQuantityAtPrice(150.0, true), 2) 
        << "Remaining buy quantity incorrect in order book";
}

TEST_F(OrderBookTest, CancelOrder) {
    Order buyOrder("O1", "T123", "AAPL", 150.0, 10, true);
    orderBook->addOrder(buyOrder);
    
    orderBook->cancelOrder("O1", true);
    EXPECT_TRUE(buyOrder.isCanceled());
    EXPECT_EQ(orderBook->getQuantityAtPrice(150.0, true), 0);
}

TEST_F(OrderBookTest, NoMatchOnPriceMismatch) {
    Order buyOrder("B1", "T1", "AAPL", 140.0, 10, true);
    Order sellOrder("S1", "T2", "AAPL", 150.0, 10, false);

    orderBook->addOrder(buyOrder);
    orderBook->addOrder(sellOrder);
    orderBook->matchOrders();

    EXPECT_EQ(buyOrder.getRemainingQuantity(), 10);
    EXPECT_EQ(sellOrder.getRemainingQuantity(), 10);
}

TEST_F(OrderBookTest, PriceTimePriorityMatching) {
    // Add orders with same price but different timestamps
    Order buyOrder1("B1", "T1", "AAPL", 150.0, 5, true);
    orderBook->addOrder(buyOrder1);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    Order buyOrder2("B2", "T2", "AAPL", 150.0, 5, true);
    orderBook->addOrder(buyOrder2);

    // Add sell order that matches both
    Order sellOrder("S1", "T3", "AAPL", 150.0, 7, false);
    orderBook->addOrder(sellOrder);

    orderBook->matchOrders();

    // First order should be fully filled, second partially
    EXPECT_EQ(buyOrder1.getRemainingQuantity(), 0);
    EXPECT_EQ(buyOrder2.getRemainingQuantity(), 3);
}

TEST_F(OrderBookTest, MultipleOrderMatching) {
    // Add multiple buy orders at different prices
    Order buyOrder1("B1", "T1", "AAPL", 155.0, 5, true);
    Order buyOrder2("B2", "T2", "AAPL", 153.0, 3, true);
    Order buyOrder3("B3", "T3", "AAPL", 151.0, 4, true);
    
    orderBook->addOrder(buyOrder1);
    orderBook->addOrder(buyOrder2);
    orderBook->addOrder(buyOrder3);

    // Add multiple sell orders
    Order sellOrder1("S1", "T4", "AAPL", 150.0, 6, false);
    Order sellOrder2("S2", "T5", "AAPL", 152.0, 4, false);
    
    orderBook->addOrder(sellOrder1);
    orderBook->addOrder(sellOrder2);

    orderBook->matchOrders();

    // Verify matching based on price-time priority
    std::cout << "After matching: buyOrder1 remaining: " << buyOrder1.getRemainingQuantity() << "\n";
    std::cout << "After matching: buyOrder2 remaining: " << buyOrder2.getRemainingQuantity() << "\n";
    std::cout << "After matching: buyOrder3 remaining: " << buyOrder3.getRemainingQuantity() << "\n";
    std::cout << "After matching: sellOrder1 remaining: " << sellOrder1.getRemainingQuantity() << "\n";
    std::cout << "After matching: sellOrder2 remaining: " << sellOrder2.getRemainingQuantity() << "\n";
    EXPECT_EQ(buyOrder1.getRemainingQuantity(), 0);
    EXPECT_EQ(buyOrder2.getRemainingQuantity(), 0);
    EXPECT_EQ(buyOrder3.getRemainingQuantity(), 4);
    EXPECT_EQ(sellOrder1.getRemainingQuantity(), 0);
    EXPECT_EQ(sellOrder2.getRemainingQuantity(), 2);
}

TEST_F(OrderBookTest, CancelAndMatch) {
    Order buyOrder1("B1", "T1", "AAPL", 151.0, 5, true);
    Order buyOrder2("B2", "T2", "AAPL", 152.0, 5, true);
    Order sellOrder("S1", "T3", "AAPL", 150.0, 8, false);

    // Add orders to the order book
    orderBook->addOrder(buyOrder1);
    orderBook->addOrder(buyOrder2);
    orderBook->addOrder(sellOrder);

    // Cancel the first buy order
    orderBook->cancelOrder("B1", true);

    // Match orders
    orderBook->matchOrders();

    // Verify that buyOrder1 was indeed canceled
    EXPECT_TRUE(orderBook->isOrderCanceled("B1", true));

    // Check quantities after matching
    // buyOrder2 should be fully matched, resulting in 0 remaining quantity
    // sellOrder should have 3 units remaining after matching with buyOrder2
    EXPECT_EQ(buyOrder2.getRemainingQuantity(), 0) << "buyOrder2 should be fully matched";
    EXPECT_EQ(sellOrder.getRemainingQuantity(), 3) << "sellOrder should have 3 remaining units";
}

TEST_F(OrderBookTest, EmptyOrderBook) {
    orderBook->matchOrders();  // Should not crash
    EXPECT_EQ(orderBook->getQuantityAtPrice(100.0, true), 0);
    EXPECT_EQ(orderBook->getQuantityAtPrice(100.0, false), 0);
}

TEST_F(OrderBookTest, MismatchedStockSymbols) {
    Order buyOrder("B1", "T1", "AAPL", 150.0, 5, true);
    Order sellOrder("S1", "T2", "MSFT", 150.0, 5, false);

    orderBook->addOrder(buyOrder);
    orderBook->addOrder(sellOrder);
    orderBook->matchOrders();

    // Orders shouldn't match due to different symbols
    EXPECT_EQ(buyOrder.getRemainingQuantity(), 5);
    EXPECT_EQ(sellOrder.getRemainingQuantity(), 5);
}

TEST_F(OrderBookTest, ZeroQuantityOrders) {
    EXPECT_THROW(Order("B1", "T1", "AAPL", 150.0, 0, true), std::invalid_argument);
    EXPECT_THROW(Order("S1", "T2", "APPL", 160.0, 0, false), std::invalid_argument);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
