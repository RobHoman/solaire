// Copyright 2016 Phil Homan
#ifndef SRC_APP_H_
#define SRC_APP_H_

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include "./app_logic.h"
#include "./app_initialization.h"

class App {
 public:
  App();
  ~App();

  bool InitInstance();
  void Run();
  bool IsRunning() { return is_running_; }
  void QuitApp() { is_quitting_ = true; }
  void SetQuitting(bool is_quitting) { is_quitting_ = is_quitting; }
  int GetExitCode() { return exit_code_; }
  AppLogic* GetAppLogic() const { return p_app_logic_; }
  const glm::ivec2 &GetScreenSize() { return window_size_; }

  AppLogic* p_app_logic_;
  AppOptions options_;
  GLFWwindow* p_window_;

 private:
  glm::ivec2 window_size_;
  bool is_running_;
  bool is_quit_requested_;
  bool is_quitting_;
  int exit_code_;
};

extern App* gp_app;

#endif  // SRC_APP_H_
