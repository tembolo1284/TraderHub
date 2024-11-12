#include <iostream>
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
    OrderClient(std::shared_ptr<Channel> channel) : stub_(OrderService::NewStub(channel)) {}

    void SubmitOrder(const std::string& order_id, const std::string& trader_id,
                     const std::string& stock_symbol, double price, int quantity, bool is_buy) {
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
            std::cout << "Order submitted: " << response.status() << std::endl;
        } else {
            std::cout << "RPC failed" << std::endl;
        }
    }

private:
    std::unique_ptr<OrderService::Stub> stub_;
};

int main() {
    OrderClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    client.SubmitOrder("order1", "trader1", "AAPL", 150.0, 10, true);
    client.SubmitOrder("order2", "trader2", "APPL", 149.0, 10, false);
    return 0;
}

