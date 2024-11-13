// src/order_service.cpp
#include "order_service.hpp"
#include <spdlog/spdlog.h>

OrderServiceImpl::OrderServiceImpl(std::shared_ptr<OrderClientServer> server)
    : server_(std::move(server)) {
    if (!server_) {
        throw std::invalid_argument("Server cannot be null");
    }
}

grpc::Status OrderServiceImpl::SubmitOrder(
    [[maybe_unused]] grpc::ServerContext* context,
    const order_service::OrderRequest* request,
    order_service::OrderResponse* response) {
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

grpc::Status OrderServiceImpl::CancelOrder(
    [[maybe_unused]] grpc::ServerContext* context,
    const order_service::CancelRequest* request,
    order_service::CancelResponse* response) {
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

grpc::Status OrderServiceImpl::ViewOrderBook(
    [[maybe_unused]] grpc::ServerContext* context,
    const order_service::ViewOrderBookRequest* request,
    order_service::ViewOrderBookResponse* response) {
    
    try {
        auto [buy_orders, sell_orders] = server_->getOrderBook(request->symbol());

        // Convert buy orders
        for (const auto& order : buy_orders) {
            auto* entry = response->add_buy_orders();
            entry->set_order_id(order.getOrderId());
            entry->set_trader_id(order.getTraderId());
            entry->set_symbol(order.getStockSymbol());  // Changed from getSymbol to getStockSymbol
            entry->set_price(order.getPrice());
            entry->set_quantity(order.getQuantity());
            entry->set_is_buy_order(true);
        }

        // Convert sell orders
        for (const auto& order : sell_orders) {
            auto* entry = response->add_sell_orders();
            entry->set_order_id(order.getOrderId());
            entry->set_trader_id(order.getTraderId());
            entry->set_symbol(order.getStockSymbol());  // Changed from getSymbol to getStockSymbol
            entry->set_price(order.getPrice());
            entry->set_quantity(order.getQuantity());
            entry->set_is_buy_order(false);
        }

        return grpc::Status::OK;
    }
    catch (const std::exception& e) {
        return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
    }
}
