// Copyright 2016 Phil Homan

#ifndef APP_APP_LOGIC_H_
#define APP_APP_LOGIC_H_

#include <memory>

#include "./clock.h"

#include "spdlog/spdlog.h"

namespace solaire {
namespace app {

class AppLogic {
 public:
  AppLogic();
  ~AppLogic();

  bool Init();
  void Update();

 private:
  std::shared_ptr<spdlog::logger> log_;

  Clock app_clock_;
  Clock frame_clock_;
};

}  // namespace app
}  // namespace solaire

#endif  // APP_APP_LOGIC_H_
