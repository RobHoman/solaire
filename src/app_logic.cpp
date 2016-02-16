// Copyright 2016 Phil Homan

#include <easylogging++.h>

#include "./app_logic.h"

AppLogic::AppLogic() {}

bool AppLogic::Init() {
  app_clock_.Init();
  frame_clock_.Init();

  LOG(INFO) << "Initialized the AppLogic";
  return true;
}

void AppLogic::Update() {
  double dt = frame_clock_.Restart();

  // TODO(Phil): Main loop logic goes here
}
