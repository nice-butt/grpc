#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>
#include <thread>

#include <grpcpp/grpcpp.h>

#include "experiments/hw1/hw1.grpc.pb.h"

namespace grpc::experiments {

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(Greeter::NewStub(channel)) {}

  std::string SayHi(const std::string& user, const std::string& message);

 private:
  std::unique_ptr<Greeter::Stub> stub_;
};

std::string GreeterClient::SayHi(const std::string& user, const std::string& message) {
  HiRequest request;
  request.set_name(user);
  std::stringstream ss;
  std::time_t time_v = std::time(nullptr);
  ss << "Hi: " << message << ", at: " << std::ctime(&time_v);
  request.set_hi_message(ss.str());

  // Receive hi_reply
  HiResponse response;
  ClientContext context;
  Status s = stub_->SayHi(&context, request, &response);
  return s.ok() ? response.hi_reply() : "RPC failed";
}

}  // namespace grpc::experiments

int main(int argc, char** argv) {
  const std::string target = "localhost:50051";
  grpc::experiments::GreeterClient greeter(grpc::CreateChannel(target, grpc::InsecureChannelCredentials()));
  while (true) {
    std::cout << greeter.SayHi("drz0", "hi until canceled") << "\n" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }
  return 0;
}
