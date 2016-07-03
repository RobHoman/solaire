// Copyright 2016 Phil Homan

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

class AppConfig {
 public:
  AppConfig();
  ~AppConfig();

  bool Init(const char* ini_file_path);
  static void LogConfig();

  #define CONFIG(section, property, default) char* section##_##property;
  #include "app_config.def"
};
#endif  // APP_CONFIG_H_
