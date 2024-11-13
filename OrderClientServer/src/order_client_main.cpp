// src/order_client_main.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <filesystem>
#include <iomanip>
#include <grpcpp/grpcpp.h>
#include "order_service.grpc.pb.h"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std::chrono_literals;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using namespace order_service;
namespace fs = std::filesystem;

class OrderClient {
public:
    OrderClient(const std::string& server_address) {
        spdlog::info("Connecting to server at {}", server_address);
        
        auto channel = grpc::CreateChannel(
            server_address, 
            grpc::InsecureChannelCredentials()
        );
        
        // Wait for connection
        if (!channel->WaitForConnected(std::chrono::system_clock::now() + 5s)) {
            throw std::runtime_error("Failed to connect to server");
        }
        
        stub_ = OrderService::NewStub(channel);
        spdlog::info("Connected to server successfully");
    }

    bool submitOrder(const std::string& order_id, 
                    const std::string& trader_id,
                    const std::string& stock_symbol, 
                    double price, 
                    int quantity, 
                    bool is_buy) {
        OrderRequest request;
        auto* details = request.mutable_details();
        details->set_order_id(order_id);
        details->set_trader_id(trader_id);
        details->set_stock_symbol(stock_symbol);
        details->set_price(price);
        details->set_quantity(quantity);
        details->set_is_buy_order(is_buy);

        OrderResponse response;
        ClientContext context;
        
        spdlog::info("Submitting order: ID={}, Symbol={}, Price={}, Qty={}, Side={}", 
                    order_id, stock_symbol, price, quantity, is_buy ? "BUY" : "SELL");

        Status status = stub_->SubmitOrder(&context, request, &response);

        if (status.ok()) {
            spdlog::info("Order submitted successfully:");
            spdlog::info("Status: {}", OrderStatus_Name(response.status()));
            if (response.status() == OrderStatus::FULLY_FILLED) {
                spdlog::info("Matched Price: {}", response.matched_price());
                spdlog::info("Matched Quantity: {}", response.matched_quantity());
            }
            return true;
        }
        
        spdlog::error("RPC failed: {}", status.error_message());
        return false;
    }

    bool cancelOrder(const std::string& order_id, bool is_buy) {
        CancelRequest request;
        request.set_order_id(order_id);
        request.set_is_buy_order(is_buy);
        request.set_trader_id("system");  // You might want to make this configurable

        CancelResponse response;
        ClientContext context;
        
        spdlog::info("Cancelling order: ID={}", order_id);

        Status status = stub_->CancelOrder(&context, request, &response);

        if (status.ok()) {
            spdlog::info("Cancel request result: {}", OrderStatus_Name(response.status()));
            return response.status() == OrderStatus::CANCELLED;
        }
        
        spdlog::error("RPC failed: {}", status.error_message());
        return false;
    }

    bool viewOrderBook(const std::string& symbol = "") {
        ViewOrderBookRequest request;
        request.set_symbol(symbol);
        
        ViewOrderBookResponse response;
        ClientContext context;
        
        spdlog::info("Requesting order book{}...", 
                     symbol.empty() ? "" : " for symbol " + symbol);

        Status status = stub_->ViewOrderBook(&context, request, &response);

        if (status.ok()) {
            // Print buy orders
            std::cout << "\nBuy Orders:\n";
            std::cout << std::string(80, '-') << "\n";
            std::cout << std::setw(12) << "Order ID" 
                      << std::setw(12) << "Trader" 
                      << std::setw(10) << "Symbol"
                      << std::setw(12) << "Price"
                      << std::setw(12) << "Quantity"
                      << std::setw(12) << "Remaining"
                      << "\n";
            std::cout << std::string(80, '-') << "\n";
            
            for (const auto& entry : response.buy_orders()) {
                const auto& details = entry.details();
                std::cout << std::setw(12) << details.order_id()
                          << std::setw(12) << details.trader_id()
                          << std::setw(10) << details.stock_symbol()
                          << std::setw(12) << std::fixed << std::setprecision(2) << details.price()
                          << std::setw(12) << details.quantity()
                          << std::setw(12) << entry.remaining_quantity()
                          << "\n";
            }

            // Print sell orders
            std::cout << "\nSell Orders:\n";
            std::cout << std::string(80, '-') << "\n";
            std::cout << std::setw(12) << "Order ID" 
                      << std::setw(12) << "Trader" 
                      << std::setw(10) << "Symbol"
                      << std::setw(12) << "Price"
                      << std::setw(12) << "Quantity"
                      << std::setw(12) << "Remaining"
                      << "\n";
            std::cout << std::string(80, '-') << "\n";
            
            for (const auto& entry : response.sell_orders()) {
                const auto& details = entry.details();
                std::cout << std::setw(12) << details.order_id()
                          << std::setw(12) << details.trader_id()
                          << std::setw(10) << details.stock_symbol()
                          << std::setw(12) << std::fixed << std::setprecision(2) << details.price()
                          << std::setw(12) << details.quantity()
                          << std::setw(12) << entry.remaining_quantity()
                          << "\n";
            }

            std::cout << "\nTotal Buy Orders: " << response.total_buy_orders()
                      << "\nTotal Sell Orders: " << response.total_sell_orders()
                      << "\nTimestamp: " << response.timestamp() << "\n";

            return true;
        }
        
        spdlog::error("RPC failed: {}", status.error_message());
        return false;
    }

    bool processOrdersFromFile(const std::string& filename) {
        try {
            // Construct path to data directory
            fs::path dataPath = fs::current_path() / "data" / filename;
            
            spdlog::info("Attempting to read orders from: {}", dataPath.string());
            
            std::ifstream file(dataPath);
            if (!file.is_open()) {
                spdlog::error("Failed to open file: {}", dataPath.string());
                return false;
            }

            json orders;
            file >> orders;

            if (!orders.contains("orders")) {
                spdlog::error("JSON file does not contain 'orders' array");
                return false;
            }

            int orderCount = 0;
            for (const auto& order : orders["orders"]) {
                try {
                    bool success = submitOrder(
                        order["order_id"].get<std::string>(),
                        order["trader_id"].get<std::string>(),
                        order["symbol"].get<std::string>(),
                        order["price"].get<double>(),
                        order["quantity"].get<int>(),
                        order["is_buy"].get<bool>()
                    );
                    if (success) {
                        orderCount++;
                    }
                    // Add small delay between orders
                    std::this_thread::sleep_for(100ms);
                }
                catch (const json::exception& e) {
                    spdlog::error("Error parsing order: {}", e.what());
                    continue;
                }
            }
            
            spdlog::info("Successfully processed {} orders from file", orderCount);
            return true;
        }
        catch (const fs::filesystem_error& e) {
            spdlog::error("Filesystem error: {}", e.what());
            return false;
        }
        catch (const json::exception& e) {
            spdlog::error("JSON parsing error: {}", e.what());
            return false;
        }
        catch (const std::exception& e) {
            spdlog::error("Error processing orders file: {}", e.what());
            return false;
        }
    }

private:
    std::unique_ptr<OrderService::Stub> stub_;
};

void printUsage() {
    std::cout << "Usage:\n"
              << "  OrderClient submit <order_id> <trader_id> <symbol> <price> <quantity> <buy/sell>\n"
              << "  OrderClient cancel <order_id> <buy/sell>\n"
              << "  OrderClient file <filename>\n"
              << "  OrderClient view [symbol]\n"
              << "\nExamples:\n"
              << "  OrderClient submit order1 trader1 AAPL 150.50 100 buy\n"
              << "  OrderClient cancel order1 buy\n"
              << "  OrderClient file orders.json    # reads from data/orders.json\n"
              << "  OrderClient view               # view all orders\n"
              << "  OrderClient view AAPL          # view orders for AAPL\n";
}

int main(int argc, char* argv[]) {
    try {
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        
        if (argc < 2) {
            printUsage();
            return 1;
        }

        OrderClient client("localhost:50051");
        std::string command = argv[1];

        if (command == "submit" && argc == 8) {
            bool result = client.submitOrder(
                argv[2],                    // order_id
                argv[3],                    // trader_id
                argv[4],                    // symbol
                std::stod(argv[5]),         // price
                std::stoi(argv[6]),         // quantity
                std::string(argv[7]) == "buy" // is_buy
            );
            return result ? 0 : 1;
        }
        else if (command == "cancel" && argc == 4) {
            bool result = client.cancelOrder(
                argv[2],                    // order_id
                std::string(argv[3]) == "buy" // is_buy
            );
            return result ? 0 : 1;
        }
        else if (command == "file" && argc == 3) {
            bool result = client.processOrdersFromFile(argv[2]);
            return result ? 0 : 1;
        }
        else if (command == "view" && (argc == 2 || argc == 3)) {
            std::string symbol = (argc == 3) ? argv[2] : "";
            bool result = client.viewOrderBook(symbol);
            return result ? 0 : 1;
        }
        else {
            printUsage();
            return 1;
        }
    }
    catch (const std::exception& e) {
        spdlog::error("Error: {}", e.what());
        return 1;
    }
}
