// Copyright 2016 Phil Homan

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

namespace solaire {
namespace app {

class AppConfig {
 public:
  AppConfig();
  ~AppConfig();

  bool Init(const char* ini_file_path);
  void LogConfig();


  #define CONFIG(section, property, type, default) type section##_##property;
  #include "app_config.def"
};
}  // namespace app
}  // namespace solaire

#endif  // APP_CONFIG_H_
