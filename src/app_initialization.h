// Copyright 2016 Phil Homan

#ifndef SRC_APP_INITIALIZATION_H_
#define SRC_APP_INITIALIZATION_H_

#include <glm/vec2.hpp>

class AppOptions {
 public:
  AppOptions();
  ~AppOptions();


  bool Init(const char* options_file_path);

  glm::ivec2 window_size_;
};
#endif  // SRC_APP_INITIALIZATION_H_
