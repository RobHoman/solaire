// Copyright 2016 Phil Homan
#ifndef APP_APP_SERVICES_H_
#define APP_APP_SERVICES_H_

#include "common/types.h"

namespace kato {
namespace app {

struct AppMemory {
  uint64 permanent_storage_size;
  uint64 transient_storage_size;
  void* permanent_storage;
  void* transient_storage;
};

struct AppState {};

struct AppKeyState {
  bool pressed;
  uint32 half_transitions;
};

struct AppInput {
  union {
    AppKeyState keys[4];
    struct {
      AppKeyState up;
      AppKeyState down;
      AppKeyState left;
      AppKeyState right;
    };
  };
};

void AppUpdateAndRender(AppMemory* memory, AppInput* input);

}  // namespace app
}  // namespace kato

#endif  // APP_APP_SERVICES_H_
