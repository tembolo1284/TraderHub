// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: order_service.proto

#include "order_service.pb.h"
#include "order_service.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace order_service {

static const char* OrderService_method_names[] = {
  "/order_service.OrderService/SubmitOrder",
  "/order_service.OrderService/CancelOrder",
  "/order_service.OrderService/ViewOrderBook",
  "/order_service.OrderService/StreamOrderBook",
};

std::unique_ptr< OrderService::Stub> OrderService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< OrderService::Stub> stub(new OrderService::Stub(channel, options));
  return stub;
}

OrderService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_SubmitOrder_(OrderService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_CancelOrder_(OrderService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_ViewOrderBook_(OrderService_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_StreamOrderBook_(OrderService_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  {}

::grpc::Status OrderService::Stub::SubmitOrder(::grpc::ClientContext* context, const ::order_service::OrderRequest& request, ::order_service::OrderResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::order_service::OrderRequest, ::order_service::OrderResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_SubmitOrder_, context, request, response);
}

void OrderService::Stub::async::SubmitOrder(::grpc::ClientContext* context, const ::order_service::OrderRequest* request, ::order_service::OrderResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::order_service::OrderRequest, ::order_service::OrderResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SubmitOrder_, context, request, response, std::move(f));
}

void OrderService::Stub::async::SubmitOrder(::grpc::ClientContext* context, const ::order_service::OrderRequest* request, ::order_service::OrderResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SubmitOrder_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::order_service::OrderResponse>* OrderService::Stub::PrepareAsyncSubmitOrderRaw(::grpc::ClientContext* context, const ::order_service::OrderRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::order_service::OrderResponse, ::order_service::OrderRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_SubmitOrder_, context, request);
}

::grpc::ClientAsyncResponseReader< ::order_service::OrderResponse>* OrderService::Stub::AsyncSubmitOrderRaw(::grpc::ClientContext* context, const ::order_service::OrderRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSubmitOrderRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status OrderService::Stub::CancelOrder(::grpc::ClientContext* context, const ::order_service::CancelRequest& request, ::order_service::CancelResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::order_service::CancelRequest, ::order_service::CancelResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_CancelOrder_, context, request, response);
}

void OrderService::Stub::async::CancelOrder(::grpc::ClientContext* context, const ::order_service::CancelRequest* request, ::order_service::CancelResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::order_service::CancelRequest, ::order_service::CancelResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_CancelOrder_, context, request, response, std::move(f));
}

void OrderService::Stub::async::CancelOrder(::grpc::ClientContext* context, const ::order_service::CancelRequest* request, ::order_service::CancelResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_CancelOrder_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::order_service::CancelResponse>* OrderService::Stub::PrepareAsyncCancelOrderRaw(::grpc::ClientContext* context, const ::order_service::CancelRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::order_service::CancelResponse, ::order_service::CancelRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_CancelOrder_, context, request);
}

::grpc::ClientAsyncResponseReader< ::order_service::CancelResponse>* OrderService::Stub::AsyncCancelOrderRaw(::grpc::ClientContext* context, const ::order_service::CancelRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncCancelOrderRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status OrderService::Stub::ViewOrderBook(::grpc::ClientContext* context, const ::order_service::ViewOrderBookRequest& request, ::order_service::ViewOrderBookResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::order_service::ViewOrderBookRequest, ::order_service::ViewOrderBookResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_ViewOrderBook_, context, request, response);
}

void OrderService::Stub::async::ViewOrderBook(::grpc::ClientContext* context, const ::order_service::ViewOrderBookRequest* request, ::order_service::ViewOrderBookResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::order_service::ViewOrderBookRequest, ::order_service::ViewOrderBookResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ViewOrderBook_, context, request, response, std::move(f));
}

void OrderService::Stub::async::ViewOrderBook(::grpc::ClientContext* context, const ::order_service::ViewOrderBookRequest* request, ::order_service::ViewOrderBookResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ViewOrderBook_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::order_service::ViewOrderBookResponse>* OrderService::Stub::PrepareAsyncViewOrderBookRaw(::grpc::ClientContext* context, const ::order_service::ViewOrderBookRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::order_service::ViewOrderBookResponse, ::order_service::ViewOrderBookRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_ViewOrderBook_, context, request);
}

::grpc::ClientAsyncResponseReader< ::order_service::ViewOrderBookResponse>* OrderService::Stub::AsyncViewOrderBookRaw(::grpc::ClientContext* context, const ::order_service::ViewOrderBookRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncViewOrderBookRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::ClientReader< ::order_service::ViewOrderBookResponse>* OrderService::Stub::StreamOrderBookRaw(::grpc::ClientContext* context, const ::order_service::ViewOrderBookRequest& request) {
  return ::grpc::internal::ClientReaderFactory< ::order_service::ViewOrderBookResponse>::Create(channel_.get(), rpcmethod_StreamOrderBook_, context, request);
}

void OrderService::Stub::async::StreamOrderBook(::grpc::ClientContext* context, const ::order_service::ViewOrderBookRequest* request, ::grpc::ClientReadReactor< ::order_service::ViewOrderBookResponse>* reactor) {
  ::grpc::internal::ClientCallbackReaderFactory< ::order_service::ViewOrderBookResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_StreamOrderBook_, context, request, reactor);
}

::grpc::ClientAsyncReader< ::order_service::ViewOrderBookResponse>* OrderService::Stub::AsyncStreamOrderBookRaw(::grpc::ClientContext* context, const ::order_service::ViewOrderBookRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::order_service::ViewOrderBookResponse>::Create(channel_.get(), cq, rpcmethod_StreamOrderBook_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::order_service::ViewOrderBookResponse>* OrderService::Stub::PrepareAsyncStreamOrderBookRaw(::grpc::ClientContext* context, const ::order_service::ViewOrderBookRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::order_service::ViewOrderBookResponse>::Create(channel_.get(), cq, rpcmethod_StreamOrderBook_, context, request, false, nullptr);
}

OrderService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      OrderService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< OrderService::Service, ::order_service::OrderRequest, ::order_service::OrderResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](OrderService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::order_service::OrderRequest* req,
             ::order_service::OrderResponse* resp) {
               return service->SubmitOrder(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      OrderService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< OrderService::Service, ::order_service::CancelRequest, ::order_service::CancelResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](OrderService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::order_service::CancelRequest* req,
             ::order_service::CancelResponse* resp) {
               return service->CancelOrder(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      OrderService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< OrderService::Service, ::order_service::ViewOrderBookRequest, ::order_service::ViewOrderBookResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](OrderService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::order_service::ViewOrderBookRequest* req,
             ::order_service::ViewOrderBookResponse* resp) {
               return service->ViewOrderBook(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      OrderService_method_names[3],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< OrderService::Service, ::order_service::ViewOrderBookRequest, ::order_service::ViewOrderBookResponse>(
          [](OrderService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::order_service::ViewOrderBookRequest* req,
             ::grpc::ServerWriter<::order_service::ViewOrderBookResponse>* writer) {
               return service->StreamOrderBook(ctx, req, writer);
             }, this)));
}

OrderService::Service::~Service() {
}

::grpc::Status OrderService::Service::SubmitOrder(::grpc::ServerContext* context, const ::order_service::OrderRequest* request, ::order_service::OrderResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status OrderService::Service::CancelOrder(::grpc::ServerContext* context, const ::order_service::CancelRequest* request, ::order_service::CancelResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status OrderService::Service::ViewOrderBook(::grpc::ServerContext* context, const ::order_service::ViewOrderBookRequest* request, ::order_service::ViewOrderBookResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status OrderService::Service::StreamOrderBook(::grpc::ServerContext* context, const ::order_service::ViewOrderBookRequest* request, ::grpc::ServerWriter< ::order_service::ViewOrderBookResponse>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace order_service

