// Copyright 2016 Phil Homan

#include "app/app_logic.h"

namespace solaire {
namespace app {

AppLogic::AppLogic() {}

bool AppLogic::Init() {
  log_ = spdlog::get("consoleAndFile");

  app_clock_.Init();
  frame_clock_.Init();

  log_->info("Initialized the AppLogic.");
  return true;
}

void AppLogic::Update() {
  double dt = frame_clock_.Restart();

  // TODO(Phil): Main loop logic goes here
}

}  // namespace app
}  // namespace solaire
