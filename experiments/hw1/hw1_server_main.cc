#include <iostream>
#include <memory>
#include <string>
#include <ctime>
#include <sstream>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "experiments/hw1/hw1.grpc.pb.h"

namespace grpc::experiments {

class GreeterServiceImpl final : public Greeter::Service {
  Status SayHi(ServerContext* context, const HiRequest* request, HiResponse* response) override;
};

Status GreeterServiceImpl::SayHi(ServerContext* context, const HiRequest* request, HiResponse* response) {
  std::stringstream ss;
  const std::time_t time_v = std::time(nullptr);
  ss << "Hi, " << request->name() << "\n"
      << "received: " << request->hi_message() << "\n"
      << "time: " << std::ctime(&time_v);
  response->set_hi_reply(ss.str());
  return Status::OK;
}

void RunServer() {
  const std::string server_address("0.0.0.0:50051");
  GreeterServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  server->Wait();
}

}  // namespace grpc::experiments

int main(int argc, char** argv) {
  grpc::experiments::RunServer();
  return 0;
}
