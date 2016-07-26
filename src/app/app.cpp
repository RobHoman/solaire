// Copyright 2016 Phil Homan

#include "app/app.h"

#include "glad/glad.h"

#include "platform_services.h"

namespace solaire {
namespace app {

void AppUpdateAndRender() {
  debug_read_file appFile = DebugPlatformReadEntireFile("rules.ninja");
  DebugPlatformWriteEntireFile("test.out", appFile.size, appFile.memory);
  DebugPlatformFreeFileMemory(appFile.size, appFile.memory);

  // Set background color.
  glClearColor(1.0, 0.0, 0.0, 1.0);
  // Clear back buffer
  glClear(GL_COLOR_BUFFER_BIT);
}

}  // namespace app
}  // namespace solaire
