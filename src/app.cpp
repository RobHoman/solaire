// Copyright 2016 Phil Homan

#include <glad/glad.h>

#include "./app.h"

#include "spdlog/spdlog.h"

namespace solaire {
namespace app {

auto log = spdlog::get("consoleAndFile");

App* gp_app = nullptr;

App::App() {
  gp_app = this;
  p_window_ = nullptr;
  p_renderer_ = nullptr;
  p_app_logic_ = new AppLogic();
  p_config_ = new AppConfig();
  window_size_ = glm::ivec2(0, 0);

  is_running_ = false;
  is_quit_requested_ = false;
  is_quitting_ = false;
  exit_code_ = false;
}

static void log_sdl_error(const char* description) {
  log->error("{} SDL_GetError: {}", description, SDL_GetError());
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
    log_sdl_error("SDL_Init failed.");
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
                       "SOLAIRE",
                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       window_size_.x, window_size_.y,
                       SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (p_window_ == nullptr) {
    log_sdl_error("SDL_CreateWindow failed.");
    return false;
  }

  SDL_GLContext gl_context = SDL_GL_CreateContext(p_window_);
  if (gl_context == nullptr) {
    log_sdl_error("SDL_GL_CreateContext failed.");
  }

  // Load all OpenGL functions using the SDL2 loader function
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    log->error("Failed to load glad GLLoader.");
    return false;
  }

  // Report the initialized GL state
  log->info("GL Vendor: {}", glGetString(GL_VENDOR));
  log->info("GL Renderer: {}", glGetString(GL_RENDERER));
  log->info("GL Version: {}", glGetString(GL_VERSION));

  if (p_config_->graphics_vsync) {
    SDL_GL_SetSwapInterval(1);
  }

  if (!p_app_logic_->Init()) {
    log->error("Failed to initialize app logic.");
    return false;
  }

  log->info("Initialized the App instance.");
  is_running_ = true;
  return true;
}

void App::Run() {
  SDL_Event event;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
      if (event.type == SDL_KEYDOWN) {
        // if the escape key is pressed, quit the application
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          quit = true;
        }
      }
      if (event.type == SDL_MOUSEBUTTONDOWN) {
        quit = true;
      }
    }
    // Set background color
    glClearColor(1.0, 0.0, 0.5, 1.0);
    // Clear back buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // Render, swapping back and front buffer
    SDL_GL_SwapWindow(p_window_);
  }
}

}  // namespace app
}  // namespace solaire
