#pragma once

#include <folly/Memory.h>
#include <folly/executors/IOThreadPoolExecutor.h>
#include <folly/executors/CPUThreadPoolExecutor.h>
#include <folly/futures/Future.h>

#include <proxygen/httpserver/RequestHandler.h>

using namespace proxygen;

namespace hello {

class HelloHandler : public RequestHandler {
public:
  explicit HelloHandler(folly::CPUThreadPoolExecutor *);

  void onRequest(std::unique_ptr<HTTPMessage> headers) noexcept override;
  void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;
  void onEOM() noexcept override;
  void onUpgrade(UpgradeProtocol protocol) noexcept override;
  void requestComplete() noexcept override;
  void onError(ProxygenError err) noexcept override;

private:
  std::unique_ptr<folly::IOBuf> body_;
  folly::Future<int> add(int x, int y);

  folly::CPUThreadPoolExecutor *cpuPool_;
};

} // namespace hello
