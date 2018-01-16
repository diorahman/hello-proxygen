#include "HelloHandler.h"

#include <folly/io/async/EventBaseManager.h>

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

using namespace proxygen;

namespace hello {

HelloHandler::HelloHandler(folly::CPUThreadPoolExecutor *cpuPool)
    : cpuPool_(cpuPool) {}

void HelloHandler::onRequest(std::unique_ptr<HTTPMessage> headers) noexcept {}

void HelloHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {}

void HelloHandler::onEOM() noexcept {
  auto evb = folly::EventBaseManager::get()->getEventBase();
  add(1, 2).via(cpuPool_).then([this, evb](int result) {
    evb->runInEventBaseThread([this, result]() {
      ResponseBuilder(downstream_)
        .status(200, "OK")
        .header("Hihi", "Hehe")
        .body("Result: " + std::to_string(result))
        .sendWithEOM();
    });
  });
}

void HelloHandler::onUpgrade(UpgradeProtocol protocol) noexcept {}

void HelloHandler::requestComplete() noexcept { delete this; }

void HelloHandler::onError(ProxygenError err) noexcept { delete this; }

folly::Future<int> HelloHandler::add(int x, int y) {
  folly::Promise<int> p;
  auto f = p.getFuture();
  p.setValue(x + y);
  return f;
}

} // namespace hello
