// Copyright 2016 Phil Homan
#ifndef APP_APP_H_
#define APP_APP_H_

#include "common/types.h"

namespace kato {
namespace app {

struct AppMemory {
  uint64 permanent_storage_size;
  uint64 transient_storage_size;
  void* permanent_storage;
  void* transient_storage;
  bool is_initialized;
};

struct AppState {
  float tone_hz;
  int16 tone_volume;
};

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

struct AppSoundOutput {
  int samples_per_second;
  int16* samples;
  uint32 samples_to_write;
  // TODO(phil): Remove running sample index?
  uint32 running_sample_index;
};

extern "C" {
void AppUpdateAndRender(AppMemory* memory, AppInput* input);
void AppGetSoundSamples(AppMemory* memory, AppSoundOutput* app_sound_output);
}
typedef void AppUpdateAndRenderFunc(AppMemory* memory, AppInput* input);
typedef void AppGetSoundSamplesFunc(AppMemory* memory,
                                    AppSoundOutput* app_sound_output);

}  // namespace app
}  // namespace kato

#endif  // APP_APP_H_
