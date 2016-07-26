// Copyright 2016 Phil Homan

#include <sys/mman.h>

#include "./platform_services.h"

#include "SDL.h"
#include "glad/glad.h"

#include "./types.h"
#include "app/app.h"

void DebugPlatformFreeFileMemory(int64 size, void* memory) {
  if (memory) {
    munmap(memory, size);
  }
}

debug_read_file DebugPlatformReadEntireFile(const char* filename) {
  debug_read_file result = {0};

  SDL_RWops* io = SDL_RWFromFile(filename, "rb");
  if (io) {
    int64 file_size = int64{SDL_RWsize(io)};
    result.memory =
        mmap(0 /* null addr */, file_size, PROT_READ | PROT_WRITE,
             MAP_ANONYMOUS | MAP_PRIVATE, -1 /* null fd */, 0 /* no offset */);
    if (result.memory != MAP_FAILED) {
      int64 bytes_read = SDL_RWread(io, result.memory, 1 /* size */, file_size);
      if (bytes_read == file_size) {
        result.size = file_size;
      } else {
        // TODO(phil): Log
        DebugPlatformFreeFileMemory(file_size, result.memory);
        result.size = 0;
      }
    } else {
      // TODO(phil): Log
    }
    SDL_RWclose(io);
  } else {
    // TODO(phil): Log
  }

  return result;
}

bool DebugPlatformWriteEntireFile(const char* filename, int64 size,
                                  void* memory) {
  bool result = false;

  SDL_RWops* io = SDL_RWFromFile(filename, "w");
  if (io) {
    if (SDL_RWwrite(io, memory, size, 1) == 1) {
      result = true;
    } else {
      // TODO(phil): Log
    }
    SDL_RWclose(io);
  }

  return result;
}

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

    window = SDL_CreateWindow("SOLAIRE", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, 1280, 1020,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window) {
      gl_context = SDL_GL_CreateContext(window);
      if (gl_context) {
        // NOTE(phil) Load all OpenGL functions using the SDL2 loader function
        if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
          globalRunning = true;
          while (globalRunning) {
            // TODO(phil): Put input handling into a separate function
            // and pass captured input along to the App.
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

              solaire::app::AppUpdateAndRender();
              // Render, swapping back and front buffer.
              SDL_GL_SwapWindow(window);
            }
          }
        } else {
          // TODO(phil) Log
          SDL_GetError();
        }
      } else {
        // TODO(phil) Log
        SDL_GetError();
      }
    } else {
      // TODO(phil) Log
      SDL_GetError();
    }

    SDL_Quit();

  } else {
    // TODO(phil) Log
    SDL_GetError();
  }

  return 1;
}
