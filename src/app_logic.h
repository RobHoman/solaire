// Copyright 2016 Phil Homan

#ifndef SRC_APP_LOGIC_H_
#define SRC_APP_LOGIC_H_

#include "./clock.h"

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
#endif  // SRC_APP_LOGIC_H_
