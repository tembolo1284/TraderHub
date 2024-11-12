// tests/order_client_server_tests.cpp
#include "order_client_server.hpp"
#include "order_types.hpp"
#include <gtest/gtest.h>

class OrderClientServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        order_book = std::make_shared<OrderBook>();
        server = std::make_shared<OrderClientServer>(order_book);
    }

    std::shared_ptr<OrderBook> order_book;
    std::shared_ptr<OrderClientServer> server;
};

TEST_F(OrderClientServerTest, SubmitOrderTest) {
    std::string orderId = "order1";
    std::string traderId = "trader1";
    std::string stockSymbol = "AAPL";
    double price = 150.0;
    int quantity = 100;
    bool isBuyOrder = true;

    MatchResult result = server->submitOrder(orderId, traderId, stockSymbol, price, quantity, isBuyOrder);
    EXPECT_FALSE(result.status.empty());
    EXPECT_GE(result.price, 0.0);
    EXPECT_GE(result.quantity, 0);
}

TEST_F(OrderClientServerTest, CancelOrderTest) {
    std::string orderId = "order1";
    std::string traderId = "trader1";
    std::string stockSymbol = "AAPL";
    double price = 150.0;
    int quantity = 100;
    bool isBuyOrder = true;

    // First submit an order
    server->submitOrder(orderId, traderId, stockSymbol, price, quantity, isBuyOrder);
    
    // Then try to cancel it
    std::string result = server->cancelOrder(orderId, isBuyOrder);
    EXPECT_FALSE(result.empty());
}
