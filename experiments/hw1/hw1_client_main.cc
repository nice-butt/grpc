#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>
#include <thread>

#include "experiments/hw1.grpc.pb.h"

namespace grpc::experiments {

class GreeterClient {
 public:
  GreeterClient(const std::shared_ptr<Channel>& channel)
      : stub_(Greeter::NewStub(channel)) {}

  std::string SayHi(const std::string& user, const std::string& message);

 private:
  std::unique_ptr<Greeter::Stub> stub_;
};

Status GreeterClient::SayHi(const std::string& user, const std::string& message) {
  HiRequest request;
  request.set_name(user);
  std::stringstream ss;
  ss << "Hi: " << message << ", at: " << std::ctime(std::time(nullptr));
  request.set_hi_message(ss.str());

  // Receive hi_reply
  HiResponse response;
  ClientContext context;
  return stub_->SayHi(&context, request, &response);
}

}  // namespace grpc::experiments

int main(int argc, char** argv) {
  const std::string target = "localhost:50051";
  GreeterClient greeter(grpc::CreateChannel(target, grpc::InsecureChannelCredentials()));
  using namespace std::chrono_literals;
  while (true) {
    greeter->SayHi("drz0", "hi until canceled");
    std::this_thread::sleep_for(2s);
  }
  return 0;
}
