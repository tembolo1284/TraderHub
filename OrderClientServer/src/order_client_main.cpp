// src/order_client_main.cpp
#include <iostream>
#include <string>
#include <grpcpp/grpcpp.h>
#include "order_service.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using order_service::OrderService;
using order_service::OrderRequest;
using order_service::OrderResponse;
using order_service::CancelRequest;
using order_service::CancelResponse;

class OrderClient {
public:
    OrderClient(std::shared_ptr<Channel> channel) 
        : stub_(OrderService::NewStub(channel)) {}

    bool SubmitOrder(const std::string& order_id, const std::string& trader_id,
                    const std::string& stock_symbol, double price, 
                    int quantity, bool is_buy) {
        OrderRequest request;
        request.set_order_id(order_id);
        request.set_trader_id(trader_id);
        request.set_stock_symbol(stock_symbol);
        request.set_price(price);
        request.set_quantity(quantity);
        request.set_is_buy_order(is_buy);

        OrderResponse response;
        ClientContext context;
        Status status = stub_->SubmitOrder(&context, request, &response);

        if (status.ok()) {
            std::cout << "Order submitted successfully!\n";
            std::cout << "Status: " << response.status() << "\n";
            std::cout << "Matched Price: " << response.matched_price() << "\n";
            std::cout << "Matched Quantity: " << response.matched_quantity() << "\n";
            return true;
        } else {
            std::cout << "RPC failed: " << status.error_message() << "\n";
            return false;
        }
    }

    bool CancelOrder(const std::string& order_id, bool is_buy) {
        CancelRequest request;
        request.set_order_id(order_id);
        request.set_is_buy_order(is_buy);

        CancelResponse response;
        ClientContext context;
        Status status = stub_->CancelOrder(&context, request, &response);

        if (status.ok()) {
            std::cout << "Cancel request result: " << response.status() << "\n";
            return true;
        } else {
            std::cout << "RPC failed: " << status.error_message() << "\n";
            return false;
        }
    }

private:
    std::unique_ptr<OrderService::Stub> stub_;
};

int main() {
    // Create client
    OrderClient client(
        grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials())
    );

    // Test submit order
    std::cout << "Submitting buy order...\n";
    client.SubmitOrder("order1", "trader1", "AAPL", 150.0, 100, true);

    std::cout << "\nSubmitting sell order...\n";
    client.SubmitOrder("order2", "trader2", "AAPL", 149.5, 110, false);

    // Test cancel order
    std::cout << "\nCancelling order...\n";
    client.CancelOrder("order2", true);

    return 0;
}
