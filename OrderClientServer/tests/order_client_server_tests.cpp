// tests/order_client_server_tests.cpp
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include "order_client_server.hpp"
#include "order_service.hpp"
#include "order_service.pb.h"
#include "order_service.grpc.pb.h"

class OrderClientServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up logging
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        spdlog::set_level(spdlog::level::debug);

        server = std::make_unique<OrderClientServer>();
    }

    void TearDown() override {
        // Cleanup
    }

    std::unique_ptr<OrderClientServer> server;

    // Helper function to create an order request
    order_service::OrderRequest createOrderRequest(
        const std::string& order_id,
        const std::string& trader_id,
        const std::string& symbol,
        double price,
        int quantity,
        bool is_buy) {
        
        order_service::OrderRequest request;
        auto* details = request.mutable_details();
        details->set_order_id(order_id);
        details->set_trader_id(trader_id);
        details->set_stock_symbol(symbol);
        details->set_price(price);
        details->set_quantity(quantity);
        details->set_is_buy_order(is_buy);
        return request;
    }
};

TEST_F(OrderClientServerTest, SubmitOrderTest) {
    std::string orderId = "order1";
    std::string traderId = "trader1";
    std::string stockSymbol = "AAPL";
    double price = 150.0;
    int quantity = 100;
    bool isBuyOrder = true;

    SPDLOG_DEBUG("Submitting order: ID={}, Trader={}, Symbol={}, Price={}, Qty={}, Side={}", 
                 orderId, traderId, stockSymbol, price, quantity, isBuyOrder ? "BUY" : "SELL");
    
    auto request = createOrderRequest(orderId, traderId, stockSymbol, price, quantity, isBuyOrder);
    auto response = server->submitOrder(request);

    EXPECT_EQ(response.status(), order_service::OrderStatus::SUCCESS);
    
    // Verify the order was added to the book
    order_service::ViewOrderBookRequest bookRequest;
    bookRequest.set_symbol(stockSymbol);
    auto orderBook = server->getOrderBook(bookRequest);
    
    EXPECT_GT(orderBook.buy_orders_size() + orderBook.sell_orders_size(), 0);
}

TEST_F(OrderClientServerTest, CancelOrderTest) {
    std::string orderId = "order1";
    std::string traderId = "trader1";
    std::string stockSymbol = "AAPL";
    double price = 150.0;
    int quantity = 100;
    bool isBuyOrder = true;

    // First submit an order
    auto submitRequest = createOrderRequest(orderId, traderId, stockSymbol, price, quantity, isBuyOrder);
    server->submitOrder(submitRequest);

    // Then try to cancel it
    order_service::CancelRequest cancelRequest;
    cancelRequest.set_order_id(orderId);
    cancelRequest.set_is_buy_order(isBuyOrder);
    cancelRequest.set_trader_id(traderId);
    
    auto response = server->cancelOrder(cancelRequest);
    EXPECT_EQ(response.status(), order_service::OrderStatus::CANCELLED);

    // Verify the order is no longer in the book
    order_service::ViewOrderBookRequest bookRequest;
    bookRequest.set_symbol(stockSymbol);
    auto orderBook = server->getOrderBook(bookRequest);
    
    EXPECT_EQ(orderBook.buy_orders_size(), 0);
}

TEST_F(OrderClientServerTest, MatchOrderTest) {
    // Submit a sell order first
    auto sellRequest = createOrderRequest(
        "sell1", "trader1", "AAPL", 100.0, 50, false);
    auto sellResponse = server->submitOrder(sellRequest);
    EXPECT_EQ(sellResponse.status(), order_service::OrderStatus::SUCCESS);

    // Submit a matching buy order
    auto buyRequest = createOrderRequest(
        "buy1", "trader2", "AAPL", 100.0, 50, true);
    auto buyResponse = server->submitOrder(buyRequest);
    EXPECT_EQ(buyResponse.status(), order_service::OrderStatus::FULLY_FILLED);
    
    // Verify order book is empty after match
    order_service::ViewOrderBookRequest bookRequest;
    auto orderBook = server->getOrderBook(bookRequest);
    EXPECT_EQ(orderBook.buy_orders_size(), 0);
    EXPECT_EQ(orderBook.sell_orders_size(), 0);
}

TEST_F(OrderClientServerTest, PartialMatchTest) {
    // Submit a sell order first
    auto sellRequest = createOrderRequest(
        "sell1", "trader1", "AAPL", 100.0, 50, false);
    auto sellResponse = server->submitOrder(sellRequest);
    EXPECT_EQ(sellResponse.status(), order_service::OrderStatus::SUCCESS);

    // Submit a larger buy order
    auto buyRequest = createOrderRequest(
        "buy1", "trader2", "AAPL", 100.0, 75, true);
    auto buyResponse = server->submitOrder(buyRequest);
    EXPECT_EQ(buyResponse.status(), order_service::OrderStatus::PARTIAL_FILL);
    
    // Verify remaining quantity in order book
    order_service::ViewOrderBookRequest bookRequest;
    auto orderBook = server->getOrderBook(bookRequest);
    EXPECT_EQ(orderBook.sell_orders_size(), 0);
    EXPECT_EQ(orderBook.buy_orders_size(), 1);
    EXPECT_EQ(orderBook.buy_orders(0).remaining_quantity(), 25);
}

TEST_F(OrderClientServerTest, PriceCrossingTest) {
    // Submit a sell order at 100
    auto sellRequest = createOrderRequest(
        "sell1", "trader1", "AAPL", 100.0, 50, false);
    server->submitOrder(sellRequest);

    // Submit a buy order at 102 (should match with sell at 100)
    auto buyRequest = createOrderRequest(
        "buy1", "trader2", "AAPL", 102.0, 50, true);
    auto buyResponse = server->submitOrder(buyRequest);
    EXPECT_EQ(buyResponse.status(), order_service::OrderStatus::FULLY_FILLED);
    
    // Verify order book is empty
    order_service::ViewOrderBookRequest bookRequest;
    auto orderBook = server->getOrderBook(bookRequest);
    EXPECT_EQ(orderBook.buy_orders_size(), 0);
    EXPECT_EQ(orderBook.sell_orders_size(), 0);
}
