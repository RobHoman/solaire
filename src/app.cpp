// Copyright 2016 Phil Homan

#include <glad/glad.h>
#include <easylogging++.h>

#include "./app.h"

App* gp_app = nullptr;

App::App() {
  gp_app = this;
  p_window_ = nullptr;
  p_app_logic_ = new AppLogic();
  window_size_ = glm::ivec2(0, 0);

  is_running_ = false;
  is_quit_requested_ = false;
  is_quitting_ = false;
  exit_code_ = false;
}

static void glfw_error_callback(int error, const char* description) {
  LOG(ERROR) << "GLFW Error {" << error << "} " << description;
}

bool App::InitInstance() {
  options_.Init("AppOptions.json");
  window_size_ = options_.window_size_;

  glfwSetErrorCallback(glfw_error_callback);

  if (!glfwInit()) {
    LOG(FATAL) << "Failed to initialize GLFW.";
    return false;
  }

  // Create a  windowed mode window and its OpenGL context
  p_window_ = glfwCreateWindow(window_size_.x, window_size_.y,
                              "Solaire", nullptr, nullptr);
  glfwMakeContextCurrent(p_window_);
  if (!p_window_) {
    LOG(FATAL) << "Failed to create app window.";
    glfwTerminate();
    return false;
  }

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    LOG(FATAL) << "Failed to initialize OpenGL context.";
    return false;
  }

  if (!p_app_logic_->Init()) {
    LOG(FATAL) << "Failed to initialize app logic.";
    return false;
  }

  LOG(INFO) << "Initialized the GraphicsApp instance";
  is_running_ = true;
  return true;
}

void App::Run() {
  // Loop until the user quits or closes the window
  while (!glfwWindowShouldClose(p_window_)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(p_window_);
    glfwPollEvents();
    gp_app->GetAppLogic()->Update();
  }
  glfwTerminate();
}
