// Copyright 2016 Phil Homan
#ifndef APP_H_
#define APP_H_

#include <glm/vec2.hpp>

#include "./app_config.h"
#include "./app_logic.h"

#include "SDL.h"

namespace solaire {
namespace app {

class App {
 public:
  App();
  ~App();

  bool InitInstance();
  void Run();
  void Shutdown();
  bool IsRunning() const { return is_running_; }
  void QuitApp() { is_quitting_ = true; }
  void SetQuitting(bool is_quitting) { is_quitting_ = is_quitting; }
  int GetExitCode() const { return exit_code_; }
  AppLogic* GetAppLogic() const { return p_app_logic_; }
  glm::ivec2 &GetScreenSize() { return window_size_; }

  AppLogic* p_app_logic_;
  AppConfig* p_config_;
  SDL_Window* p_window_;
  SDL_Renderer* p_renderer_;

 private:
  glm::ivec2 window_size_;
  bool is_running_;
  bool is_quit_requested_;
  bool is_quitting_;
  int exit_code_;
};

extern App* gp_app;

}  // namespace app
}  // namespace solaire

#endif  // APP_H_
