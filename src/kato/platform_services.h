// Copyright 2016 Phil Homan

#ifndef KATO_PLATFORM_SERVICES_H_
#define KATO_PLATFORM_SERVICES_H_

#include <time.h>

#include "common/types.h"
#include "app/app_services.h"

struct LinuxState {
  char* base_path;
  int base_path_length;
};

struct LinuxAppLib {
  void* lib_handle;
  kato::app::AppUpdateAndRenderFunc* update_and_render;
  kato::app::AppGetSoundSamplesFunc* get_sound_samples;

  time_t modify_time;
};

struct DebugReadFile {
  uint64 size;
  void* memory;
};

void DebugPlatformFreeFileMemory(int64 size, void* memory);
DebugReadFile DebugPlatformReadEntireFile(const char* filename);
bool DebugPlatformWriteEntireFile(const char* filename, int64 size,
                                  void* memory);

#endif  // KATO_PLATFORM_SERVICES_H_
