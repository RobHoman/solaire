// Copyright 2016 phil homan

#include "./app_config.h"

#include <cstdio>

static auto log = spdlog::get("consoleandfile");

/* process a line of the ini file, storing valid values */
int ini_handler(void *user, const char* ini_section, const char* ini_property,
                const char *value) {
  // Empty if to allow macro expansion to use else ifs
  if (0) {}
  #define CONFIG(section, property, default) \
    else if (stricmp(ini_section, #section) == 0 && \
             stricmp(ini_property, #property) == 0) section##_##property = strdup(value);
  #include "app_config.def"

  return 1;
}

AppConfig::AppConfig() {
  // set all of the config variables to reasonable defaults
  #define CONFIG(section, property, default) section##_##property = default;
  #include "app_config.def"
}

AppConfig::~AppConfig() {}

bool AppConfig::init(const char* ini_file_path) {
  if (ini_parse(ini_file_path, handler, &config) < 0)
    printf("can't load 'test.ini', using defaults\n");
  dump_config(&config);
  return 0;
}

/* print all the variables in the config, one per line */
static void AppConfig::LogConfig() {
  log->info("Logging the config variables and values:");
    #define CONFIG(section, property, default) log->info("{}_{} = {}", #section, #property, section##_##property);
    #include "app_config.def"
}
