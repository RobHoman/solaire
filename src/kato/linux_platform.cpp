// Copyright 2016 Phil Homan

#include <sys/mman.h>

#include "kato/platform_services.h"

#include "SDL.h"
#include "glad/glad.h"

#include "app/app_services.h"
#include "common/macros.h"
#include "common/types.h"

void DebugPlatformFreeFileMemory(int64 size, void* memory) {
  if (memory) {
    munmap(memory, size);
  }
}

DebugReadFile DebugPlatformReadEntireFile(const char* filename) {
  DebugReadFile result = {};

  SDL_RWops* io = SDL_RWFromFile(filename, "rb");
  if (io) {
    int64 file_size = (int64)SDL_RWsize(io);
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

void ProcessKeyEvent(kato::app::AppKeyState* key, bool pressed) {
  if (key->pressed != pressed) {
    key->pressed = pressed;
    key->half_transitions++;
  }
}

int main(int argc, char* argv[]) {
  bool globalRunning = false;
  SDL_Window* window;
  SDL_GLContext gl_context;

  // NOTE(phil)Initialize SDL2 with the default subsystems
  // and the subsystems specified as flags.
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == 0) {
    // NOTE(phil) Setup the minimum required OpenGL context attributes
    // TODO(phil) Determine the best values for this GL attrbiutes.
    // Possibly make these easily configurable.
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

    window =
        SDL_CreateWindow("KATO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         1280, 1020, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window) {
      gl_context = SDL_GL_CreateContext(window);
      if (gl_context) {
        // NOTE(phil) Load all OpenGL functions using the SDL2 loader function
        if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
#if KATO_INTERNAL
          void* memory_base_address = (void*)TERABYTES(2);
#else
          void* memory_base_address = (void*)0;
#endif
          kato::app::AppMemory app_memory = {};
          app_memory.permanent_storage_size = MEGABYTES(64);
          app_memory.transient_storage_size = GIGABYTES(1);

          app_memory.permanent_storage =
              mmap(memory_base_address, app_memory.permanent_storage_size +
                                            app_memory.transient_storage_size,
                   PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
          app_memory.transient_storage = (uint8*)app_memory.permanent_storage +
                                         app_memory.permanent_storage_size;

          globalRunning = true;
          kato::app::AppInput input[2] = {};
          kato::app::AppInput* new_input = &input[0];
          kato::app::AppInput* last_input = &input[1];
          while (globalRunning) {
            // TODO(phil): For each button copy the pressed state. Zero the
            // transition counts somehow.
            // TODO(phil): Figure out why the pressed state isn't persisting
            // between frames.
            // *new_input = {};
            for (int key_index = 0; key_index < ARRAY_COUNT(new_input->keys);
                 ++key_index) {
              new_input->keys[key_index].pressed =
                  last_input->keys[key_index].pressed;
            }

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
              switch (event.type) {
                case SDL_QUIT: {
                  globalRunning = false;
                  break;
                }
                case SDL_KEYDOWN:
                case SDL_KEYUP: {
                  bool pressed = (event.type == SDL_KEYDOWN);
                  switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: {
                      globalRunning = false;
                      break;
                    }
                    case SDLK_w: {
                      ProcessKeyEvent(&new_input->up, pressed);
                      break;
                    }
                    case SDLK_a: {
                      ProcessKeyEvent(&new_input->left, pressed);
                      break;
                    }
                    case SDLK_s: {
                      ProcessKeyEvent(&new_input->down, pressed);
                      break;
                    }
                    case SDLK_d: {
                      ProcessKeyEvent(&new_input->right, pressed);
                      break;
                    }
                  }
                }
              }
            }

            kato::app::AppUpdateAndRender(&app_memory, new_input);
            // NOTE(phil): Render, swapping back and front buffer.
            SDL_GL_SwapWindow(window);

            kato::app::AppInput* temp = last_input;
            last_input = new_input;
            new_input = temp;
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
