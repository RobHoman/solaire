// Copyright 2016 Phil Homan

#include "./app_initialization.h"

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include <cstdio>

using rapidjson::FileReadStream;
using rapidjson::Document;

AppOptions::AppOptions() {
  // Set all of the options to reasonable defaults
  window_size_ = glm::ivec2(800, 600);
}

AppOptions::~AppOptions() {}

bool AppOptions::Init(const char* options_file_path) {
  FILE* p_options_file = fopen(options_file_path, "rb");
  char file_read_buffer[65536];
  FileReadStream file_read_stream(p_options_file, file_read_buffer,
                               sizeof(file_read_buffer));

  Document options_doc;
  options_doc.ParseStream(file_read_stream);
  return true;
}
