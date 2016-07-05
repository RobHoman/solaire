// Copyright 2016 phil homan

#include "app/app_config.h"

#include <cstdio>
#include <cstring>
#include <iostream>

#include "ini.h"

namespace solaire {
namespace app {

static auto log = spdlog::get("consoleAndFile");

// Define each type parser to easily call
// the right function via macro token concatenation.

// Simple char* to int
int parse_int(const char* value) { return std::stoi(value); }

// Only allow case insensitive "true" and "false".
// Default to false otherwise.
bool parse_bool(const char* value) {
  if (strcasecmp(value, "true") == 0) {
    return true;
  } else if (strcasecmp(value, "false") == 0) {
    return false;
  }
  return false;
}

/* process a line of the ini file, storing valid values */
int ini_handler(void* user, const char* ini_section, const char* ini_property,
                const char* value) {
  AppConfig* appConfig = (AppConfig*)user;
  // Empty if to allow macro expansion to use else ifs
  if (0) {
  }
#define CONFIG(section, property, type, default)     \
  else if (strcasecmp(ini_section, #section) == 0 && \
           strcasecmp(ini_property, #property) == 0) \
      appConfig->section##_##property = parse##_##type(value);
#include "app_config.def"

  return 1;
}

AppConfig::AppConfig() {
  log_ = spdlog::get("consoleAndFile");

// set all of the config variables to reasonable defaults
#define CONFIG(section, property, type, default) section##_##property = default;
#include "app_config.def"
}

AppConfig::~AppConfig() {}

bool AppConfig::Init(const char* ini_file_path) {
  if (ini_parse(ini_file_path, ini_handler, this) < 0)
    log_->warn("Can't load app config '{}'. Using defaults", ini_file_path);
  LogConfig();
  return 0;
}

/* print all the variables in the config, one per line */
void AppConfig::LogConfig() {
  log_->info("Logging the config variables and values:");
#define CONFIG(section, property, type, default) \
  log_->info("{}_{} = {}", #section, #property, section##_##property);
#include "app_config.def"
}

}  // namespace app
}  // namespace solaire
