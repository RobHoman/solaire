// Copyright 2016 Phil Homan

#include "./app_initialization.h"

AppOptions::AppOptions() {
  // Set all of the options to reasonable defaults
  window_size_ = glm::ivec2(800, 600);
}

AppOptions::~AppOptions() {}

bool AppOptions::Init(const char* options_file_path) {
  return true;
}
