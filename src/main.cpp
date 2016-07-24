// Copyright 2016 Phil Homan

#include "SDL.h"

int main(int argc, char* argv[]) {
  bool globalRunning = false;
  SDL_Window* window;
  SDL_GLContext gl_context;

  // NOTE(phil)Initialize SDL2 with the default subsystems
  // and the subsystems specified as flags.
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == 0) {
    // NOTE(phil) Setup the minimum required OpenGL context attributes
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // NOTE(phil) Use the OpenGL core profile
    // with no deprecated functions available
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    // TODO(phil): Figure out how to determine what version of OpenGL to use
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ?);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ?);

    if (window = SDL_CreateWindow("SOLAIRE", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, 1280, 1020,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)) {
      if (gl_context = SDL_GL_CreateContext(p_window_)) {
        // NOTE(phil) Load all OpenGL functions using the SDL2 loader function
        if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
          globalRunning = true;
          while (globalRunning) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
              if (event.type == SDL_QUIT) {
                globalRunning = false;
              }
              if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                  globalRunning = false;
                }
              }
              // Set background color.
              glClearColor(1.0, 0.0, 0.5, 1.0);
              // Clear back buffer
              glClear(GL_COLOR_BUFFER_BIT);
              // Render, swapping back and front buffer.
              SDL_GL_SwapWindow(window);
            }
          }
        } else {
          // TODO(phil) Logging
        }
      } else {
        // TODO(phil) Logging
      }
    } else {
      // TODO(phil) Logging
    }
  } else {
    // TODO(phil) Logging
  }

  return 1;
}
