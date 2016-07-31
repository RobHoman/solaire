// Copyright 2016 Phil Homan

#ifndef KATO_PLATFORM_SERVICES_H_
#define KATO_PLATFORM_SERVICES_H_

#include "common/types.h"

struct DebugReadFile {
  uint64 size;
  void* memory;
};

void DebugPlatformFreeFileMemory(int64 size, void* memory);
DebugReadFile DebugPlatformReadEntireFile(const char* filename);
bool DebugPlatformWriteEntireFile(const char* filename, int64 size,
                                  void* memory);

#endif  // KATO_PLATFORM_SERVICES_H_
