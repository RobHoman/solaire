// Copyright 2016 Phil Homan

#ifndef APP_LOGIC_H_
#define APP_LOGIC_H_

#include "./clock.h"

namespace solaire {
namespace app {

class AppLogic {
 public:
  AppLogic();
  ~AppLogic();

  bool Init();
  void Update();

 private:
  Clock app_clock_;
  Clock frame_clock_;
};

}  // namespace app
}  // namespace solaire

#endif  // APP_LOGIC_H_
