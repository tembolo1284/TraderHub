#include <gtest/gtest.h>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <sstream>
#include "order_book.hpp"
#include "trader.hpp"
#include "trade.hpp"
#include "order.hpp"

class PerformanceTest : public ::testing::Test {
protected:
    PerformanceTest() 
        : orderBook(), rd(), gen(rd()), price_dist(90.0, 110.0), quantity_dist(1, 100),
          next_buy_order_id(1), next_sell_order_id(1) {}

    void SetUp() override {
        orderBook = std::make_unique<OrderBook>();
    }

    // Helper function to measure execution time
    template<typename Func>
    std::chrono::nanoseconds measureExecutionTime(Func&& func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }

    void printPerformanceMetrics(const std::string& testName, 
                               std::size_t n, 
                               std::chrono::nanoseconds duration,
                               bool includeLogN = true) {
        double milliseconds = static_cast<double>(duration.count()) / 1e6;
        double seconds = milliseconds / 1000.0;
        double opsPerSecond = static_cast<double>(n) / seconds;
        double timePerOp = milliseconds / static_cast<double>(n);
        double logN = std::log2(static_cast<double>(n));
        double timePerLogN = timePerOp / logN;

        std::cout << "\n" << testName << " Performance Metrics:" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Number of operations: " << n << std::endl;
        std::cout << "Total time: " << milliseconds << " ms" << std::endl;
        std::cout << "Operations per second: " << std::fixed << std::setprecision(2) 
                  << opsPerSecond << std::endl;
        std::cout << "Time per operation: " << timePerOp << " ms" << std::endl;
        if (includeLogN) {
            std::cout << "Time per operation / log(n): " << timePerLogN << " ms" << std::endl;
        }
        std::cout << "----------------------------------------" << std::endl;
    }

    // Test helper functions
    Order generateRandomOrder(bool isBuyOrder) {
        std::string prefix = isBuyOrder ? "B" : "S";
        int orderNumber = isBuyOrder ? next_buy_order_id++ : next_sell_order_id++;
        
        return Order(
            prefix + std::to_string(orderNumber),
            "T" + std::to_string(orderNumber % 100),
            "AAPL",
            price_dist(gen),
            quantity_dist(gen),
            isBuyOrder
        );
    }

    std::unique_ptr<OrderBook> orderBook;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<double> price_dist;
    std::uniform_int_distribution<int> quantity_dist;
    int next_buy_order_id;
    int next_sell_order_id;
};

TEST_F(PerformanceTest, OrderInsertionScaling) {
    const std::vector<std::size_t> sizes = {1000, 5000, 10000, 50000, 100000};
    
    std::cout << "\nOrder Insertion Scaling Test" << std::endl;
    std::cout << "Size\tTime(ms)\tTime/n(μs)\tTime/nlogn(μs)" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    for (std::size_t n : sizes) {
        std::vector<Order> orders;
        orders.reserve(n);
        
        for (std::size_t i = 0; i < n; ++i) {
            orders.push_back(generateRandomOrder(i % 2 == 0));
        }

        auto duration = measureExecutionTime([&]() {
            for (auto& order : orders) {
                orderBook->addOrder(order);
            }
        });

        double milliseconds = static_cast<double>(duration.count()) / 1e6;
        double microsPerOp = (milliseconds * 1000.0) / static_cast<double>(n);
        double logN = std::log2(static_cast<double>(n));
        double microsPerLogN = microsPerOp / logN;

        std::cout << n << "\t" 
                  << std::fixed << std::setprecision(2) << milliseconds << "\t\t"
                  << microsPerOp << "\t\t"
                  << microsPerLogN << std::endl;

        orderBook = std::make_unique<OrderBook>();
    }
}

TEST_F(PerformanceTest, OrderMatchingPerformance) {
    const std::vector<std::size_t> sizes = {1000, 10000, 25000, 50000, 100000, 1000000};

    std::cout << "\nOrder Matching Performance Test" << std::endl;
    std::cout << "Size\tMatch Time(ms)\tTime/n(μs)" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    for (std::size_t n : sizes) {
        std::cout << "Starting test for size: " << n << std::endl;

        // Adding orders to the order book
        for (std::size_t i = 0; i < n; ++i) {
            auto order = generateRandomOrder(i % 2 == 0);
            orderBook->addOrder(order);

            if (i > 0 && i % 250000 == 0) {
                std::cout << "Added " << i << " orders to the order book." << std::endl;
            }
        }
        std::cout << "Finished adding " << n << " orders." << std::endl;

        // Measure matching time
        auto duration = measureExecutionTime([&]() {
            orderBook->matchOrders();
        });

        double milliseconds = static_cast<double>(duration.count()) / 1e6;
        double microsPerOp = (milliseconds * 1000.0) / static_cast<double>(n);

        std::cout << "Matching completed for size: " << n << std::endl;
        std::cout << n << "\t"
                  << std::fixed << std::setprecision(2) << milliseconds << "\t\t"
                  << microsPerOp << std::endl;

        // Reset the order book for the next iteration
        orderBook = std::make_unique<OrderBook>();
        std::cout << "Order book reset completed for next test size.\n" << std::endl;
    }
}

TEST_F(PerformanceTest, ConcurrentOrderProcessing) {
    constexpr std::size_t numOrders = 10000;
    constexpr std::size_t batchSize = 1000;
    
    std::vector<Order> buyOrders;
    std::vector<Order> sellOrders;
    buyOrders.reserve(numOrders);
    sellOrders.reserve(numOrders);

    for (std::size_t i = 0; i < numOrders; ++i) {
        buyOrders.push_back(generateRandomOrder(true));
        sellOrders.push_back(generateRandomOrder(false));
    }

    auto duration = measureExecutionTime([&]() {
        for (std::size_t i = 0; i < numOrders; i += batchSize) {
            for (std::size_t j = 0; j < batchSize && (i + j) < numOrders; ++j) {
                orderBook->addOrder(buyOrders[i + j]);
                orderBook->addOrder(sellOrders[i + j]);
            }
            orderBook->matchOrders();
        }
    });

    printPerformanceMetrics("Concurrent Order Processing", 
                           numOrders * 2, // Total number of orders processed
                           duration,
                           false);
}

TEST_F(PerformanceTest, OrderCancellationPerformance) {
    constexpr std::size_t numOrders = 10000;
    std::vector<std::pair<std::string, bool>> orderIds; // pair of (orderId, isBuyOrder)
    orderIds.reserve(numOrders);

    // Add orders and store their IDs
    for (std::size_t i = 0; i < numOrders; ++i) {
        bool isBuyOrder = i % 2 == 0;
        auto order = generateRandomOrder(isBuyOrder);
        orderIds.emplace_back(order.getOrderId(), isBuyOrder);
        orderBook->addOrder(order);
    }

    auto duration = measureExecutionTime([&]() {
        for (const auto& [orderId, isBuyOrder] : orderIds) {
            orderBook->cancelOrder(orderId, isBuyOrder);
        }
    });

    printPerformanceMetrics("Order Cancellation", numOrders, duration);
}

TEST_F(PerformanceTest, MixedOperationsPerformance) {
    constexpr std::size_t numOperations = 10000;
    std::vector<Order> orders;
    orders.reserve(numOperations);

    // Generate initial orders
    for (std::size_t i = 0; i < numOperations; ++i) {
        orders.push_back(generateRandomOrder(i % 2 == 0));
    }

    auto duration = measureExecutionTime([&]() {
        for (std::size_t i = 0; i < numOperations; ++i) {
            if (i % 3 == 0) {
                // Add new order
                orderBook->addOrder(orders[i]);
            } else if (i % 3 == 1) {
                // Cancel an order
                if (!orders.empty()) {
                    const auto& order = orders[i % orders.size()];
                    orderBook->cancelOrder(order.getOrderId(), order.isBuyOrder());
                }
            } else {
                // Match orders
                orderBook->matchOrders();
            }
        }
    });

    printPerformanceMetrics("Mixed Operations", numOperations, duration, false);
}

TEST_F(PerformanceTest, HighFrequencyTrading) {
    constexpr std::size_t numOrders = 100000;
    constexpr std::size_t batchSize = 100;
    
    // Prepare orders
    std::vector<Order> orders;
    orders.reserve(numOrders);
    
    for (std::size_t i = 0; i < numOrders; ++i) {
        orders.push_back(generateRandomOrder(i % 2 == 0));
    }

    auto duration = measureExecutionTime([&]() {
        for (std::size_t i = 0; i < numOrders; i += batchSize) {
            // Process a batch of orders
            for (std::size_t j = 0; j < batchSize && (i + j) < numOrders; ++j) {
                orderBook->addOrder(orders[i + j]);
            }
            
            // Match after each batch
            orderBook->matchOrders();
            
            // Cancel oldest orders periodically
            if (i >= batchSize * 10) {
                for (std::size_t k = 0; k < batchSize / 2; ++k) {
                    const auto& oldOrder = orders[i - batchSize * 10 + k];
                    orderBook->cancelOrder(oldOrder.getOrderId(), oldOrder.isBuyOrder());
                }
            }
        }
    });

    printPerformanceMetrics("High Frequency Trading Simulation", 
                           numOrders, 
                           duration, 
                           true);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    std::cout << std::fixed << std::setprecision(2);
    return RUN_ALL_TESTS();
}
