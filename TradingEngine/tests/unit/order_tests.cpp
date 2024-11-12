// tests/unit/order_tests.cpp
#include <gtest/gtest.h>
#include "order.hpp"
#include <thread>
#include <chrono>

class OrderTest : public ::testing::Test {
protected:
    // Initialize member in constructor
    OrderTest() 
        : order()  // Initialize unique_ptr to nullptr
    {}

    void SetUp() override {
        order = std::make_unique<Order>("O1", "T123", "AAPL", 150.5, 10, true);
    }

    std::unique_ptr<Order> order;
};

TEST_F(OrderTest, CreateOrder) {
    EXPECT_EQ(order->getOrderId(), "O1");
    EXPECT_EQ(order->getTraderId(), "T123");
    EXPECT_EQ(order->getStockSymbol(), "AAPL");
    EXPECT_DOUBLE_EQ(order->getPrice(), 150.5);
    EXPECT_EQ(order->getQuantity(), 10);
    EXPECT_TRUE(order->isBuyOrder());
}

TEST_F(OrderTest, OrderEquality) {
    // Create two orders with identical fields
    Order order1("O1", "T123", "AAPL", 150.5, 10, true);
    
    // Test each field individually first
    SCOPED_TRACE("Testing individual fields");
    {
        Order order2("O1", "T123", "AAPL", 150.5, 10, true);
        
        EXPECT_EQ(order1.getOrderId(), order2.getOrderId()) << "OrderId mismatch";
        EXPECT_EQ(order1.getTraderId(), order2.getTraderId()) << "TraderId mismatch";
        EXPECT_EQ(order1.getStockSymbol(), order2.getStockSymbol()) << "StockSymbol mismatch";
        EXPECT_DOUBLE_EQ(order1.getPrice(), order2.getPrice()) << "Price mismatch";
        EXPECT_EQ(order1.getQuantity(), order2.getQuantity()) << "Quantity mismatch";
        EXPECT_EQ(order1.isBuyOrder(), order2.isBuyOrder()) << "BuyOrder flag mismatch";
        EXPECT_EQ(order1.getRemainingQuantity(), order2.getRemainingQuantity()) << "RemainingQuantity mismatch";
        
        // Now test full object equality
        EXPECT_TRUE(order1 == order2) << "Full object comparison failed";
    }

    // Test inequality with different fields
    SCOPED_TRACE("Testing inequalities");
    {
        // Different orderId
        Order diffOrderId("O2", "T123", "AAPL", 150.5, 10, true);
        EXPECT_FALSE(order1 == diffOrderId) << "Different OrderId should not be equal";

        // Different traderId
        Order diffTraderId("O1", "T999", "AAPL", 150.5, 10, true);
        EXPECT_FALSE(order1 == diffTraderId) << "Different TraderId should not be equal";

        // Different symbol
        Order diffSymbol("O1", "T123", "MSFT", 150.5, 10, true);
        EXPECT_FALSE(order1 == diffSymbol) << "Different Symbol should not be equal";

        // Different price
        Order diffPrice("O1", "T123", "AAPL", 160.5, 10, true);
        EXPECT_FALSE(order1 == diffPrice) << "Different Price should not be equal";

        // Different quantity
        Order diffQuantity("O1", "T123", "AAPL", 150.5, 20, true);
        EXPECT_FALSE(order1 == diffQuantity) << "Different Quantity should not be equal";

        // Different side (buy/sell)
        Order diffSide("O1", "T123", "AAPL", 150.5, 10, false);
        EXPECT_FALSE(order1 == diffSide) << "Different Side should not be equal";
    }

    // Test self-equality
    SCOPED_TRACE("Testing self-equality");
    {
        EXPECT_TRUE(order1 == order1) << "Order should be equal to itself";
    }
}

TEST_F(OrderTest, ReduceQuantity) {
    EXPECT_NO_THROW(order->reduceQuantity(5));
    EXPECT_EQ(order->getRemainingQuantity(), 5);
    
    EXPECT_THROW(order->reduceQuantity(10), std::invalid_argument);
}

TEST_F(OrderTest, CancelOrder) {
    EXPECT_FALSE(order->isCanceled());
    order->cancel();
    EXPECT_TRUE(order->isCanceled());
    EXPECT_EQ(order->getRemainingQuantity(), order->getQuantity());
}

TEST_F(OrderTest, PriceTimePriority) {
    // Test buy orders with different prices
    {
        Order lowerPriceBuy("O1", "T1", "AAPL", 100.0, 10, true);
        Order higherPriceBuy("O2", "T2", "AAPL", 101.0, 10, true);
        
        // Higher price buy order should have priority (be "less than")
        EXPECT_LT(higherPriceBuy, lowerPriceBuy) 
            << "Higher price buy order should have priority";
    }

    // Test sell orders with different prices
    {
        Order lowerPriceSell("O3", "T3", "AAPL", 100.0, 10, false);
        Order higherPriceSell("O4", "T4", "AAPL", 101.0, 10, false);
        
        // Lower price sell order should have priority (be "less than")
        EXPECT_LT(lowerPriceSell, higherPriceSell) 
            << "Lower price sell order should have priority";
    }

    // Test timestamp priority for equal prices (buy orders)
    {
        Order firstBuy("O5", "T5", "AAPL", 100.0, 10, true);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        Order secondBuy("O6", "T6", "AAPL", 100.0, 10, true);
        
        // Earlier timestamp should have priority
        EXPECT_LT(firstBuy, secondBuy) 
            << "Earlier timestamp should have priority for equal prices";
    }

    // Test timestamp priority for equal prices (sell orders)
    {
        Order firstSell("O7", "T7", "AAPL", 100.0, 10, false);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        Order secondSell("O8", "T8", "AAPL", 100.0, 10, false);
        
        // Earlier timestamp should have priority
        EXPECT_LT(firstSell, secondSell) 
            << "Earlier timestamp should have priority for equal prices";
    }
}


TEST_F(OrderTest, OrderComparison) {
    // Test buy orders
    {
        Order firstOrder("O1", "T1", "AAPL", 100.0, 10, true);
        Order higherPriceOrder("O3", "T3", "AAPL", 101.0, 10, true);

        // Debug output
        std::cout << "\nBuy Order Comparison:" << std::endl;
        std::cout << "First Order: Price=" << firstOrder.getPrice() 
                  << ", Time=" << firstOrder.getTimestamp()
                  << ", Buy=" << firstOrder.isBuyOrder() << std::endl;
        std::cout << "Higher Price Order: Price=" << higherPriceOrder.getPrice()
                  << ", Time=" << higherPriceOrder.getTimestamp()
                  << ", Buy=" << higherPriceOrder.isBuyOrder() << std::endl;

        // For buy orders, higher price should have priority
        auto cmp = firstOrder <=> higherPriceOrder;
        std::cout << "Comparison result: " 
                  << (cmp < 0 ? "less" : (cmp > 0 ? "greater" : "equal")) 
                  << std::endl;

        EXPECT_GT(firstOrder, higherPriceOrder) 
            << "Buy order with higher price (101.0) should have higher priority than lower price (100.0)";
    }

    // Test sell orders
    {
        Order firstOrder("O4", "T4", "AAPL", 100.0, 10, false);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        Order higherPriceOrder("O5", "T5", "AAPL", 101.0, 10, false);

        // Debug output
        std::cout << "\nSell Order Comparison:" << std::endl;
        std::cout << "First Order: Price=" << firstOrder.getPrice()
                  << ", Time=" << firstOrder.getTimestamp()
                  << ", Buy=" << firstOrder.isBuyOrder() << std::endl;
        std::cout << "Higher Price Order: Price=" << higherPriceOrder.getPrice()
                  << ", Time=" << higherPriceOrder.getTimestamp()
                  << ", Buy=" << higherPriceOrder.isBuyOrder() << std::endl;

        // For sell orders, lower price should have priority
        auto cmp = firstOrder <=> higherPriceOrder;
        std::cout << "Comparison result: "
                  << (cmp < 0 ? "less" : (cmp > 0 ? "greater" : "equal"))
                  << std::endl;

        EXPECT_LT(firstOrder, higherPriceOrder)
            << "Sell order with lower price (100.0) should have higher priority than higher price (101.0)";
    }

    // Test equal prices
    {
        Order earlier("O6", "T6", "AAPL", 100.0, 10, true);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        Order later("O7", "T7", "AAPL", 100.0, 10, true);

        // Debug output
        std::cout << "\nEqual Price Comparison:" << std::endl;
        std::cout << "Earlier Order: Price=" << earlier.getPrice()
                  << ", Time=" << earlier.getTimestamp()
                  << ", Buy=" << earlier.isBuyOrder() << std::endl;
        std::cout << "Later Order: Price=" << later.getPrice()
                  << ", Time=" << later.getTimestamp()
                  << ", Buy=" << later.isBuyOrder() << std::endl;

        // Earlier timestamp should have priority
        auto cmp = earlier <=> later;
        std::cout << "Comparison result: "
                  << (cmp < 0 ? "less" : (cmp > 0 ? "greater" : "equal"))
                  << std::endl;

        EXPECT_LT(earlier, later)
            << "Order with earlier timestamp should have priority when prices are equal";
    }
}

TEST_F(OrderTest, PriceTimePriorityEdgeCases) {
    // Test equal orders
    {
        Order order1("O1", "T1", "AAPL", 100.0, 10, true);
        Order order2("O2", "T2", "AAPL", 100.0, 10, true);
        
        // Compare timestamps directly since prices are equal
        if (order1.getTimestamp() < order2.getTimestamp()) {
            EXPECT_LT(order1, order2) << "Earlier timestamp should have priority";
        } else {
            EXPECT_GT(order1, order2) << "Later timestamp should have lower priority";
        }
    }

    // Test zero price orders
    {
        Order zeroPriceBuy("O3", "T3", "AAPL", 0.0, 10, true);
        Order regularBuy("O4", "T4", "AAPL", 100.0, 10, true);
        
        EXPECT_GT(zeroPriceBuy, regularBuy) 
            << "Zero price buy order should have lowest priority";
        
        Order zeroPriceSell("O5", "T5", "AAPL", 0.0, 10, false);
        Order regularSell("O6", "T6", "AAPL", 100.0, 10, false);
        
        EXPECT_LT(zeroPriceSell, regularSell) 
            << "Zero price sell order should have highest priority";
    }

    // Test extreme price differences
    {
        Order lowBuy("O7", "T7", "AAPL", 1.0, 10, true);
        Order highBuy("O8", "T8", "AAPL", 1000.0, 10, true);
        
        EXPECT_LT(highBuy, lowBuy) 
            << "High price buy order should have higher priority";
        
        Order lowSell("O9", "T9", "AAPL", 1.0, 10, false);
        Order highSell("O10", "T10", "AAPL", 1000.0, 10, false);
        
        EXPECT_LT(lowSell, highSell) 
            << "Low price sell order should have higher priority";
    }
}

// Add a test for consistent ordering
TEST_F(OrderTest, PriceTimePriorityTransitivity) {
    // Test buy orders transitivity
    {
        Order order1("O1", "T1", "AAPL", 100.0, 10, true);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        Order order2("O2", "T2", "AAPL", 101.0, 10, true);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        Order order3("O3", "T3", "AAPL", 102.0, 10, true);
        
        // Check transitivity for buy orders (higher price = higher priority)
        EXPECT_LT(order3, order2) << "102 should have priority over 101";
        EXPECT_LT(order2, order1) << "101 should have priority over 100";
        EXPECT_LT(order3, order1) << "102 should have priority over 100";
    }

    // Test sell orders transitivity
    {
        Order order1("O4", "T4", "AAPL", 100.0, 10, false);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        Order order2("O5", "T5", "AAPL", 101.0, 10, false);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        Order order3("O6", "T6", "AAPL", 102.0, 10, false);
        
        // Check transitivity for sell orders (lower price = higher priority)
        EXPECT_LT(order1, order2) << "100 should have priority over 101";
        EXPECT_LT(order2, order3) << "101 should have priority over 102";
        EXPECT_LT(order1, order3) << "100 should have priority over 102";
    }
}

// Add a test for timestamp priority with equal prices
TEST_F(OrderTest, TimestampPriorityWithEqualPrices) {
    // Test buy orders with equal prices
    {
        Order firstBuy("O1", "T1", "AAPL", 100.0, 10, true);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        Order secondBuy("O2", "T2", "AAPL", 100.0, 10, true);

        std::cout << "\nBuy Orders Comparison:" << std::endl
                  << "First Buy Order: " << std::endl
                  << "  Price: " << firstBuy.getPrice() << std::endl
                  << "  Time: " << firstBuy.getTimestamp() << std::endl
                  << "Second Buy Order: " << std::endl
                  << "  Price: " << secondBuy.getPrice() << std::endl
                  << "  Time: " << secondBuy.getTimestamp() << std::endl;

        EXPECT_LT(firstBuy, secondBuy) 
            << "Earlier timestamp should have priority for equal buy prices";
    }

    // Test sell orders with equal prices
    {
        Order firstSell("O3", "T3", "AAPL", 100.0, 10, false);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        Order secondSell("O4", "T4", "AAPL", 100.0, 10, false);

        std::cout << "\nSell Orders Comparison:" << std::endl
                  << "First Sell Order: " << std::endl
                  << "  Price: " << firstSell.getPrice() << std::endl
                  << "  Time: " << firstSell.getTimestamp() << std::endl
                  << "Second Sell Order: " << std::endl
                  << "  Price: " << secondSell.getPrice() << std::endl
                  << "  Time: " << secondSell.getTimestamp() << std::endl;

        EXPECT_LT(firstSell, secondSell)
            << "Earlier timestamp should have priority for equal sell prices";
    }

    // Test mixed buy/sell orders with equal prices
    {
        std::cout << "\nPreparing mixed order test..." << std::endl;
        
        Order buyOrder("O5", "T5", "AAPL", 100.0, 10, true);
        std::cout << "Created buy order at time: " << buyOrder.getTimestamp() << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        Order sellOrder("O6", "T6", "AAPL", 100.0, 10, false);
        std::cout << "Created sell order at time: " << sellOrder.getTimestamp() << std::endl;

        std::cout << "\nMixed Order Comparison:" << std::endl
                  << "Buy Order: " << std::endl
                  << "  Price: " << buyOrder.getPrice() << std::endl
                  << "  Time: " << buyOrder.getTimestamp() << std::endl
                  << "  Is Buy: " << buyOrder.isBuyOrder() << std::endl
                  << "Sell Order: " << std::endl
                  << "  Price: " << sellOrder.getPrice() << std::endl
                  << "  Time: " << sellOrder.getTimestamp() << std::endl
                  << "  Is Buy: " << sellOrder.isBuyOrder() << std::endl;

        // When prices are equal, earlier timestamp should win regardless of buy/sell
        EXPECT_LT(buyOrder, sellOrder) 
            << "Earlier timestamp should have priority regardless of order type";
    }
}
