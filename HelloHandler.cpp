#include "HelloHandler.h"

#include <folly/io/async/EventBaseManager.h>

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

using namespace proxygen;

namespace hello {

HelloHandler::HelloHandler() {}

void HelloHandler::onRequest(std::unique_ptr<HTTPMessage> headers) noexcept {}

void HelloHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {}

void HelloHandler::onEOM() noexcept {
  auto f = add(1, 2).then([this](int result) {
    ResponseBuilder(downstream_)
        .status(200, "OK")
        .header("Hihi", "Hehe")
        .body(std::to_string(result))
        .sendWithEOM();
  });
}

void HelloHandler::onUpgrade(UpgradeProtocol protocol) noexcept {}

void HelloHandler::requestComplete() noexcept { delete this; }

void HelloHandler::onError(ProxygenError err) noexcept { delete this; }

folly::Future<int> HelloHandler::add(int x, int y) {
  folly::Promise<int> p;
  auto f = p.getFuture();
  folly::EventBaseManager::get()->getExistingEventBase()->runInLoop(
      [ p = std::move(p), x, y ]() mutable { p.setValue(x + y); });

  return f;
}

} // namespace hello
