// include/order_service.hpp
#ifndef ORDER_SERVICE_HPP
#define ORDER_SERVICE_HPP

#include "order_service.grpc.pb.h"  // Must come before other includes
#include "order_client_server.hpp"
#include <grpcpp/grpcpp.h>
#include <memory>

using namespace order_service;  // Add this to use the generated types

class OrderServiceImpl final : public OrderService::Service {
public:
    explicit OrderServiceImpl(std::shared_ptr<OrderClientServer> server);
    
    grpc::Status SubmitOrder(grpc::ServerContext* context,
                            const OrderRequest* request,
                            OrderResponse* response) override;
                             
    grpc::Status CancelOrder(grpc::ServerContext* context,
                            const CancelRequest* request,
                            CancelResponse* response) override;
private:
    std::shared_ptr<OrderClientServer> server_;
};

#endif // ORDER_SERVICE_HPP
