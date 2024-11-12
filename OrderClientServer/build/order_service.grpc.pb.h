// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: order_service.proto
#ifndef GRPC_order_5fservice_2eproto__INCLUDED
#define GRPC_order_5fservice_2eproto__INCLUDED

#include "order_service.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace order_service {

class OrderService final {
 public:
  static constexpr char const* service_full_name() {
    return "order_service.OrderService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status SubmitOrder(::grpc::ClientContext* context, const ::order_service::OrderRequest& request, ::order_service::OrderResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::order_service::OrderResponse>> AsyncSubmitOrder(::grpc::ClientContext* context, const ::order_service::OrderRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::order_service::OrderResponse>>(AsyncSubmitOrderRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::order_service::OrderResponse>> PrepareAsyncSubmitOrder(::grpc::ClientContext* context, const ::order_service::OrderRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::order_service::OrderResponse>>(PrepareAsyncSubmitOrderRaw(context, request, cq));
    }
    virtual ::grpc::Status CancelOrder(::grpc::ClientContext* context, const ::order_service::CancelRequest& request, ::order_service::CancelResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::order_service::CancelResponse>> AsyncCancelOrder(::grpc::ClientContext* context, const ::order_service::CancelRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::order_service::CancelResponse>>(AsyncCancelOrderRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::order_service::CancelResponse>> PrepareAsyncCancelOrder(::grpc::ClientContext* context, const ::order_service::CancelRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::order_service::CancelResponse>>(PrepareAsyncCancelOrderRaw(context, request, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      virtual void SubmitOrder(::grpc::ClientContext* context, const ::order_service::OrderRequest* request, ::order_service::OrderResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void SubmitOrder(::grpc::ClientContext* context, const ::order_service::OrderRequest* request, ::order_service::OrderResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      virtual void CancelOrder(::grpc::ClientContext* context, const ::order_service::CancelRequest* request, ::order_service::CancelResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void CancelOrder(::grpc::ClientContext* context, const ::order_service::CancelRequest* request, ::order_service::CancelResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::order_service::OrderResponse>* AsyncSubmitOrderRaw(::grpc::ClientContext* context, const ::order_service::OrderRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::order_service::OrderResponse>* PrepareAsyncSubmitOrderRaw(::grpc::ClientContext* context, const ::order_service::OrderRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::order_service::CancelResponse>* AsyncCancelOrderRaw(::grpc::ClientContext* context, const ::order_service::CancelRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::order_service::CancelResponse>* PrepareAsyncCancelOrderRaw(::grpc::ClientContext* context, const ::order_service::CancelRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status SubmitOrder(::grpc::ClientContext* context, const ::order_service::OrderRequest& request, ::order_service::OrderResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::order_service::OrderResponse>> AsyncSubmitOrder(::grpc::ClientContext* context, const ::order_service::OrderRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::order_service::OrderResponse>>(AsyncSubmitOrderRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::order_service::OrderResponse>> PrepareAsyncSubmitOrder(::grpc::ClientContext* context, const ::order_service::OrderRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::order_service::OrderResponse>>(PrepareAsyncSubmitOrderRaw(context, request, cq));
    }
    ::grpc::Status CancelOrder(::grpc::ClientContext* context, const ::order_service::CancelRequest& request, ::order_service::CancelResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::order_service::CancelResponse>> AsyncCancelOrder(::grpc::ClientContext* context, const ::order_service::CancelRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::order_service::CancelResponse>>(AsyncCancelOrderRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::order_service::CancelResponse>> PrepareAsyncCancelOrder(::grpc::ClientContext* context, const ::order_service::CancelRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::order_service::CancelResponse>>(PrepareAsyncCancelOrderRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void SubmitOrder(::grpc::ClientContext* context, const ::order_service::OrderRequest* request, ::order_service::OrderResponse* response, std::function<void(::grpc::Status)>) override;
      void SubmitOrder(::grpc::ClientContext* context, const ::order_service::OrderRequest* request, ::order_service::OrderResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
      void CancelOrder(::grpc::ClientContext* context, const ::order_service::CancelRequest* request, ::order_service::CancelResponse* response, std::function<void(::grpc::Status)>) override;
      void CancelOrder(::grpc::ClientContext* context, const ::order_service::CancelRequest* request, ::order_service::CancelResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
     private:
      friend class Stub;
      explicit async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::order_service::OrderResponse>* AsyncSubmitOrderRaw(::grpc::ClientContext* context, const ::order_service::OrderRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::order_service::OrderResponse>* PrepareAsyncSubmitOrderRaw(::grpc::ClientContext* context, const ::order_service::OrderRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::order_service::CancelResponse>* AsyncCancelOrderRaw(::grpc::ClientContext* context, const ::order_service::CancelRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::order_service::CancelResponse>* PrepareAsyncCancelOrderRaw(::grpc::ClientContext* context, const ::order_service::CancelRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_SubmitOrder_;
    const ::grpc::internal::RpcMethod rpcmethod_CancelOrder_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status SubmitOrder(::grpc::ServerContext* context, const ::order_service::OrderRequest* request, ::order_service::OrderResponse* response);
    virtual ::grpc::Status CancelOrder(::grpc::ServerContext* context, const ::order_service::CancelRequest* request, ::order_service::CancelResponse* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_SubmitOrder : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_SubmitOrder() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_SubmitOrder() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SubmitOrder(::grpc::ServerContext* /*context*/, const ::order_service::OrderRequest* /*request*/, ::order_service::OrderResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestSubmitOrder(::grpc::ServerContext* context, ::order_service::OrderRequest* request, ::grpc::ServerAsyncResponseWriter< ::order_service::OrderResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_CancelOrder : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_CancelOrder() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_CancelOrder() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status CancelOrder(::grpc::ServerContext* /*context*/, const ::order_service::CancelRequest* /*request*/, ::order_service::CancelResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestCancelOrder(::grpc::ServerContext* context, ::order_service::CancelRequest* request, ::grpc::ServerAsyncResponseWriter< ::order_service::CancelResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_SubmitOrder<WithAsyncMethod_CancelOrder<Service > > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_SubmitOrder : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_SubmitOrder() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::order_service::OrderRequest, ::order_service::OrderResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::order_service::OrderRequest* request, ::order_service::OrderResponse* response) { return this->SubmitOrder(context, request, response); }));}
    void SetMessageAllocatorFor_SubmitOrder(
        ::grpc::MessageAllocator< ::order_service::OrderRequest, ::order_service::OrderResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::order_service::OrderRequest, ::order_service::OrderResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_SubmitOrder() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SubmitOrder(::grpc::ServerContext* /*context*/, const ::order_service::OrderRequest* /*request*/, ::order_service::OrderResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* SubmitOrder(
      ::grpc::CallbackServerContext* /*context*/, const ::order_service::OrderRequest* /*request*/, ::order_service::OrderResponse* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithCallbackMethod_CancelOrder : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_CancelOrder() {
      ::grpc::Service::MarkMethodCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::order_service::CancelRequest, ::order_service::CancelResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::order_service::CancelRequest* request, ::order_service::CancelResponse* response) { return this->CancelOrder(context, request, response); }));}
    void SetMessageAllocatorFor_CancelOrder(
        ::grpc::MessageAllocator< ::order_service::CancelRequest, ::order_service::CancelResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(1);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::order_service::CancelRequest, ::order_service::CancelResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_CancelOrder() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status CancelOrder(::grpc::ServerContext* /*context*/, const ::order_service::CancelRequest* /*request*/, ::order_service::CancelResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* CancelOrder(
      ::grpc::CallbackServerContext* /*context*/, const ::order_service::CancelRequest* /*request*/, ::order_service::CancelResponse* /*response*/)  { return nullptr; }
  };
  typedef WithCallbackMethod_SubmitOrder<WithCallbackMethod_CancelOrder<Service > > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_SubmitOrder : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_SubmitOrder() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_SubmitOrder() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SubmitOrder(::grpc::ServerContext* /*context*/, const ::order_service::OrderRequest* /*request*/, ::order_service::OrderResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_CancelOrder : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_CancelOrder() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_CancelOrder() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status CancelOrder(::grpc::ServerContext* /*context*/, const ::order_service::CancelRequest* /*request*/, ::order_service::CancelResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_SubmitOrder : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_SubmitOrder() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_SubmitOrder() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SubmitOrder(::grpc::ServerContext* /*context*/, const ::order_service::OrderRequest* /*request*/, ::order_service::OrderResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestSubmitOrder(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_CancelOrder : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_CancelOrder() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_CancelOrder() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status CancelOrder(::grpc::ServerContext* /*context*/, const ::order_service::CancelRequest* /*request*/, ::order_service::CancelResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestCancelOrder(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_SubmitOrder : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_SubmitOrder() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->SubmitOrder(context, request, response); }));
    }
    ~WithRawCallbackMethod_SubmitOrder() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SubmitOrder(::grpc::ServerContext* /*context*/, const ::order_service::OrderRequest* /*request*/, ::order_service::OrderResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* SubmitOrder(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_CancelOrder : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_CancelOrder() {
      ::grpc::Service::MarkMethodRawCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->CancelOrder(context, request, response); }));
    }
    ~WithRawCallbackMethod_CancelOrder() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status CancelOrder(::grpc::ServerContext* /*context*/, const ::order_service::CancelRequest* /*request*/, ::order_service::CancelResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* CancelOrder(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_SubmitOrder : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_SubmitOrder() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::order_service::OrderRequest, ::order_service::OrderResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::order_service::OrderRequest, ::order_service::OrderResponse>* streamer) {
                       return this->StreamedSubmitOrder(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_SubmitOrder() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status SubmitOrder(::grpc::ServerContext* /*context*/, const ::order_service::OrderRequest* /*request*/, ::order_service::OrderResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedSubmitOrder(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::order_service::OrderRequest,::order_service::OrderResponse>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_CancelOrder : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_CancelOrder() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::StreamedUnaryHandler<
          ::order_service::CancelRequest, ::order_service::CancelResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::order_service::CancelRequest, ::order_service::CancelResponse>* streamer) {
                       return this->StreamedCancelOrder(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_CancelOrder() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status CancelOrder(::grpc::ServerContext* /*context*/, const ::order_service::CancelRequest* /*request*/, ::order_service::CancelResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedCancelOrder(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::order_service::CancelRequest,::order_service::CancelResponse>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_SubmitOrder<WithStreamedUnaryMethod_CancelOrder<Service > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_SubmitOrder<WithStreamedUnaryMethod_CancelOrder<Service > > StreamedService;
};

}  // namespace order_service


#endif  // GRPC_order_5fservice_2eproto__INCLUDED