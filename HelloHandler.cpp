#include "HelloHandler.h"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

using namespace proxygen;

namespace hello {

HelloHandler::HelloHandler() {}

void HelloHandler::onRequest(std::unique_ptr<HTTPMessage> headers) noexcept {}

void HelloHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {}

void HelloHandler::onEOM() noexcept {
  ResponseBuilder(downstream_)
      .status(200, "OK")
      .header("Hihi", "Hehe")
      .body("Hoho")
      .sendWithEOM();
}

void HelloHandler::onUpgrade(UpgradeProtocol protocol) noexcept {}

void HelloHandler::requestComplete() noexcept { delete this; }

void HelloHandler::onError(ProxygenError err) noexcept { delete this; }

} // namespace hello
