// Copyright 2016 Phil Homan

#include "app/app_services.h"

#include "glad/glad.h"

#include "common/macros.h"
#include "kato/platform_services.h"

namespace kato {
namespace app {

void AppUpdateAndRender(AppMemory* memory, AppInput* input) {
  // ASSERT(sizeof(AppState) <= memory->permanent_storage_size);
  // AppState* app_state = (AppState*)memory->permanent_storage;

  DebugReadFile appFile = DebugPlatformReadEntireFile("rules.ninja");
  DebugPlatformWriteEntireFile("test.out", appFile.size, appFile.memory);
  DebugPlatformFreeFileMemory(appFile.size, appFile.memory);

  float red = 0.0;
  float green = 0.0;
  float blue = 0.0;
  if (input->up.pressed) {
    red = 1.0;
  }
  if (input->down.pressed) {
    green = 1.0;
  }
  if (input->left.pressed) {
    blue = 1.0;
  }
  if (input->right.pressed) {
    red = 1.0;
    green = 1.0;
    blue = 1.0;
  }

  glClearColor(red, green, blue, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

}  // namespace app
}  // namespace kato
