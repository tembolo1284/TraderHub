// src/order_service.cpp
#include "order_service.hpp"
#include <spdlog/spdlog.h>

OrderServiceImpl::OrderServiceImpl(std::shared_ptr<OrderClientServer> server)
    : server_(std::move(server)) {
    if (!server_) {
        throw std::invalid_argument("Server cannot be null");
    }
}

grpc::Status OrderServiceImpl::SubmitOrder([[maybe_unused]] grpc::ServerContext* context,
                                         const OrderRequest* request,
                                         OrderResponse* response) {
    try {
        if (!request) {
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Null request");
        }

        auto result = server_->submitOrder(
            request->order_id(),
            request->trader_id(),
            request->stock_symbol(),
            request->price(),
            request->quantity(),
            request->is_buy_order()
        );

        response->set_status(result.status);
        response->set_matched_price(result.price);
        response->set_matched_quantity(result.quantity);

        return grpc::Status::OK;
    }
    catch (const OrderError& e) {
        return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
    }
    catch (const std::exception& e) {
        return grpc::Status(grpc::StatusCode::INTERNAL, "Internal error");
    }
}

grpc::Status OrderServiceImpl::CancelOrder([[maybe_unused]] grpc::ServerContext* context,
                                         const CancelRequest* request,
                                         CancelResponse* response) {
    try {
        if (!request) {
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Null request");
        }

        auto status = server_->cancelOrder(request->order_id(), request->is_buy_order());
        response->set_status(status);
        
        return grpc::Status::OK;
    }
    catch (const std::exception& e) {
        return grpc::Status(grpc::StatusCode::INTERNAL, "Internal error");
    }
}
