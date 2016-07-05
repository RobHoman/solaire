// Copyright 2016 Phil Homan

#include <glad/glad.h>

#include "app/app.h"

#include "spdlog/spdlog.h"

namespace solaire {
namespace app {

App* gp_app = nullptr;

App::App()
    : p_app_logic_(new AppLogic()),
      p_config_(new AppConfig()),
      p_window_(nullptr),
      p_renderer_(nullptr),
      window_size_(0, 0),
      is_running_(false),
      is_quitting_(false),
      exit_code_(-1),
      log_(spdlog::get("consoleAndFile")) {
  gp_app = this;
}

App::~App() {}

void App::LogSdlError(const char* description) {
  log_->error("{} SDL_GetError: {}", description, SDL_GetError());
}
void App::Shutdown() {
  if (p_window_ != nullptr) {
    SDL_DestroyWindow(p_window_);
  }
  if (p_renderer_ != nullptr) {
    SDL_DestroyRenderer(p_renderer_);
  }
  SDL_Quit();
}

bool App::InitInstance() {
  p_config_->Init("AppConfig.ini");
  window_size_.x = p_config_->window_x;
  window_size_.y = p_config_->window_y;

  // Initialize SDL2 with the default subsystems
  // and the subsystems specified as flags.
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    LogSdlError("SDL_Init failed.");
    return false;
  }

  // Setup the minimum required OpenGL context attributes
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // Use the OpenGL core profile with no deprecated functions available
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // TODO(phil): Figure out how to determine what version of OpenGL to use
  // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ?);
  // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ?);

  p_window_ = SDL_CreateWindow(
      "SOLAIRE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_size_.x,
      window_size_.y, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (p_window_ == nullptr) {
    LogSdlError("SDL_CreateWindow failed.");
    return false;
  }

  SDL_GLContext gl_context = SDL_GL_CreateContext(p_window_);
  if (gl_context == nullptr) {
    LogSdlError("SDL_GL_CreateContext failed.");
  }

  // Load all OpenGL functions using the SDL2 loader function
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    log_->error("Failed to load glad GLLoader.");
    return false;
  }

  // Report the initialized GL state
  log_->info("GL Vendor: {}", glGetString(GL_VENDOR));
  log_->info("GL Renderer: {}", glGetString(GL_RENDERER));
  log_->info("GL Version: {}", glGetString(GL_VERSION));

  if (p_config_->graphics_vsync) {
    SDL_GL_SetSwapInterval(1);
  }

  if (!p_app_logic_->Init()) {
    log_->error("Failed to initialize AppLogic.");
    return false;
  }

  log_->info("Initialized the App instance.");
  is_running_ = true;
  return true;
}

void App::Run() {
  // Poll the input events and process.
  SDL_Event event;
  while (!is_quitting_) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        is_quitting_ = true;
      }
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          is_quitting_ = true;
        }
      }
    }

    // Process graphics.
    // Set background color.
    glClearColor(1.0, 0.0, 0.5, 1.0);
    // Clear back buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // Render, swapping back and front buffer.
    SDL_GL_SwapWindow(p_window_);

    if (is_quitting_) is_running_ = false;
  }
}

}  // namespace app
}  // namespace solaire
