// Copyright 2016 Phil Homan

#ifndef APP_APP_CONFIG_H_
#define APP_APP_CONFIG_H_

#include <memory>
#include "spdlog/spdlog.h"
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

 private:
  std::shared_ptr<spdlog::logger> log_;
};
}  // namespace app
}  // namespace solaire

#endif  // APP_APP_CONFIG_H_
