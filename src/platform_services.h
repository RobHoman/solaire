// Copyright 2016 Phil Homan

#ifndef PLATFORM_SERVICES_H_
#define PLATFORM_SERVICES_H_

#include "./types.h"

struct debug_read_file {
  int64 size;
  void* memory;
};

void DebugPlatformFreeFileMemory(int64 size, void* memory);
debug_read_file DebugPlatformReadEntireFile(const char* filename);
bool DebugPlatformWriteEntireFile(const char* filename, int64 size,
                                  void* memory);

#endif  // PLATFORM_SERVICES_H_
