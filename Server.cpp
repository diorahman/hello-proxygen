#include <iostream>
#include <unistd.h>

#include <folly/Memory.h>
#include <folly/io/async/EventBaseManager.h>

#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>

#include "HelloHandler.h"

using namespace proxygen;
using namespace hello;

class HelloHandlerFactory : public RequestHandlerFactory {
public:
  void onServerStart(folly::EventBase *evb) noexcept override {}

  void onServerStop() noexcept override {}

  RequestHandler *onRequest(RequestHandler *, HTTPMessage *) noexcept override {
    return new HelloHandler();
  }
};

int main(int argc, char *argv[]) {
  int threads = sysconf(_SC_NPROCESSORS_ONLN);

  std::vector<HTTPServer::IPConfig> IPs = {
      {folly::SocketAddress("localhost", 3000, true),
       HTTPServer::Protocol::HTTP}};

  HTTPServerOptions options;
  options.threads = threads;
  options.idleTimeout = std::chrono::milliseconds(60000);
  options.shutdownOn = {SIGINT, SIGTERM};
  options.enableContentCompression = false;
  options.handlerFactories =
      RequestHandlerChain().addThen<HelloHandlerFactory>().build();

  HTTPServer server(std::move(options));
  server.bind(IPs);

  std::thread t([&]() { server.start(); });

  t.join();

  return 0;
}
