// Copyright 2016 Phil Homan

#include "app/app_services.h"

#include "glad/glad.h"

#include "common/macros.h"
#include "kato/platform_services.h"

namespace kato {
namespace app {

void AppUpdateAndRender(AppMemory* memory, AppInput* input) {
  ASSERT(sizeof(AppState) <= memory->permanent_storage_size);
  AppState* app_state = (AppState*)memory->permanent_storage;

  if (!memory->is_initialized) {
    // TODO(phil): Remove reading and writing a file once these functions are
    // used elsewhere. Keeping them for now in order to always make sure the
    // functions are "tested".
    DebugReadFile appFile = DebugPlatformReadEntireFile("rules.ninja");
    DebugPlatformWriteEntireFile("test.out", appFile.size, appFile.memory);
    DebugPlatformFreeFileMemory(appFile.size, appFile.memory);

    app_state->tone_hz = 256.f;
    app_state->tone_volume = 3000;

    memory->is_initialized = true;
  }

  float red = 0.0;
  float green = 0.0;
  float blue = 0.0;
  if (input->up.pressed) {
    app_state->tone_hz++;
    red = 1.0;
  }
  if (input->down.pressed) {
    app_state->tone_hz--;
    green = 1.0;
  }
  if (input->left.pressed) {
    app_state->tone_volume -= 20;
    blue = 1.0;
  }
  if (input->right.pressed) {
    app_state->tone_volume += 20;
    red = 0.5;
    green = 0.2;
    blue = 0.7;
  }

  glClearColor(red, green, blue, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void AppOutputSound(AppSoundOutput* app_sound_output, float tone_hz,
                    int16 tone_volume) {
  int samples_per_second = app_sound_output->samples_per_second;

  int square_wave_period = samples_per_second / tone_hz;
  int half_square_wave_period = square_wave_period / 2;

  int16* sound_sample = app_sound_output->samples;
  uint32 sample_count = app_sound_output->samples_to_write;
  for (int sample_index = 0; sample_index < sample_count; ++sample_index) {
    int16 sample_value =
        ((app_sound_output->running_sample_index++ / half_square_wave_period) %
         2)
            ? tone_volume
            : -tone_volume;
    *sound_sample++ = sample_value;
    *sound_sample++ = sample_value;
  }
}

void AppGetSoundSamples(AppMemory* memory, AppSoundOutput* app_sound_output) {
  ASSERT(sizeof(AppState) <= memory->permanent_storage_size);
  AppState* app_state = (AppState*)memory->permanent_storage;
  AppOutputSound(app_sound_output, app_state->tone_hz, app_state->tone_volume);
}

}  // namespace app
}  // namespace kato
