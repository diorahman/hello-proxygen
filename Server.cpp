#include <unistd.h>

#include <folly/Memory.h>
#include <folly/executors/IOThreadPoolExecutor.h>
#include <folly/io/async/EventBaseManager.h>

#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>

#include "HelloHandler.h"

using namespace proxygen;
using namespace hello;

class HelloHandlerFactory : public RequestHandlerFactory {
public:
  HelloHandlerFactory(size_t size)
      : cpuPool_(std::make_unique<folly::CPUThreadPoolExecutor>(size)) {}

  ~HelloHandlerFactory() {}

  void onServerStart(folly::EventBase *evb) noexcept override {}

  void onServerStop() noexcept override {}

  RequestHandler *onRequest(RequestHandler *, HTTPMessage *) noexcept override {
    return new HelloHandler(cpuPool_.get());
  }

protected:
  std::unique_ptr<folly::CPUThreadPoolExecutor> cpuPool_;
};

int main(int argc, char *argv[]) {
  size_t threads = sysconf(_SC_NPROCESSORS_ONLN);

  std::vector<HTTPServer::IPConfig> IPs = {
      {folly::SocketAddress("localhost", 3000, true),
       HTTPServer::Protocol::HTTP}};

  HTTPServerOptions options;
  options.threads = threads;
  options.idleTimeout = std::chrono::milliseconds(60000);
  options.shutdownOn = {SIGINT, SIGTERM};
  options.enableContentCompression = false;
  options.handlerFactories =
      RequestHandlerChain()
          .addThen<HelloHandlerFactory>(threads)
          .build();

  HTTPServer server(std::move(options));
  server.bind(IPs);

  std::thread t([&]() { server.start(); });

  t.join();

  return 0;
}
