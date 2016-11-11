// Copyright 2016 Phil Homan

#ifndef KATO_LINUX_PLATFORM_H_
#define KATO_LINUX_PLATFORM_H_

#include <time.h>

#include "app/app.h"
#include "common/types.h"

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

struct DebugImage {
  int w;
  int h;
  void* pixels;
};

struct DebugReadFile {
  uint64 size;
  void* memory;
};

void DebugPlatformFreeFileMemory(int64 size, void* memory);
DebugReadFile DebugPlatformReadEntireFile(const char* filename);
bool DebugPlatformWriteEntireFile(const char* filename, int64 size,
                                  void* memory);

#endif  // KATO_LINUX_PLATFORM_H_
