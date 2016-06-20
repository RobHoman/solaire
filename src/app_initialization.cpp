// Copyright 2016 Phil Homan

#include "./app_initialization.h"

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include <cstdio>

#include "cpputils/file.h"

static auto log = spdlog::get("consoleAndFile");

using rapidjson::FileReadStream;
using rapidjson::Document;

AppOptions::AppOptions() {
  // Set all of the options to reasonable defaults
  window_size_ = glm::ivec2(800, 600);
}

AppOptions::~AppOptions() {}

bool AppOptions::Init(const char* options_file_path) {
  auto options_json = cpputils::file::get_contents(file_path);
  if (options_json.empty()) {
    return false;
  }
  auto options_doc = Document.Parse(options_json);
  return true;
}
