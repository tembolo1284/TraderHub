// include/order_service.hpp
#ifndef ORDER_SERVICE_HPP
#define ORDER_SERVICE_HPP

#include "order_service.grpc.pb.h"
#include "order_client_server.hpp"
#include <grpcpp/grpcpp.h>
#include <memory>

class OrderServiceImpl final : public order_service::OrderService::Service {
public:
    explicit OrderServiceImpl(std::shared_ptr<OrderClientServer> server);
    
    grpc::Status SubmitOrder(grpc::ServerContext* context,
                            const order_service::OrderRequest* request,
                            order_service::OrderResponse* response) override;
                             
    grpc::Status CancelOrder(grpc::ServerContext* context,
                            const order_service::CancelRequest* request,
                            order_service::CancelResponse* response) override;

    grpc::Status ViewOrderBook(grpc::ServerContext* context,
                             const order_service::ViewOrderBookRequest* request,
                             order_service::ViewOrderBookResponse* response) override;
private:
    std::shared_ptr<OrderClientServer> server_;
};

#endif // ORDER_SERVICE_HPP
