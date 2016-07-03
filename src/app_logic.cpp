// Copyright 2016 Phil Homan

#include "./app_logic.h"

#include "spdlog/spdlog.h"

namespace solaire {
namespace app {

static auto log = spdlog::get("consoleAndFile");

AppLogic::AppLogic() {}

bool AppLogic::Init() {
  app_clock_.Init();
  frame_clock_.Init();

  log->info("Initialized the AppLogic.");
  return true;
}

void AppLogic::Update() {
  double dt = frame_clock_.Restart();

  // TODO(Phil): Main loop logic goes here
}

}  // namespace app
}  // namespace solaire
