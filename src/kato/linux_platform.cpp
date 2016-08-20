// Copyright 2016 Phil Homan

#include <sys/mman.h>
#include <x86intrin.h>

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

SDL_AudioDeviceID SDLInitAudio(int samples_per_second,
                               uint32 buffer_size_in_samples) {
  // TODO(phil): How to assert that buffer_size_in_samples is a power of 2?

  SDL_AudioSpec desired_audio_spec;
  SDL_AudioSpec obtained_audio_spec;
  SDL_AudioDeviceID audio_device_id;

  SDL_memset(&desired_audio_spec, 0, sizeof(desired_audio_spec));
  desired_audio_spec.freq = samples_per_second;
  desired_audio_spec.format = AUDIO_S16LSB;
  desired_audio_spec.channels = 2;
  desired_audio_spec.samples = buffer_size_in_samples;
  desired_audio_spec.callback = 0;
  audio_device_id = SDL_OpenAudioDevice(
      0 /* device */, 0 /* iscapture */, &desired_audio_spec,
      &obtained_audio_spec, 0 /* allowedChanges */);

  ASSERT(obtained_audio_spec.freq == samples_per_second);
  ASSERT(obtained_audio_spec.format == AUDIO_S16LSB);
  ASSERT(obtained_audio_spec.channels == 2);
  ASSERT(obtained_audio_spec.samples == buffer_size_in_samples);
  ASSERT(obtained_audio_spec.size ==
         buffer_size_in_samples * desired_audio_spec.channels * sizeof(int16));

  SDL_PauseAudioDevice(audio_device_id, 0);

  return audio_device_id;
}

int main(int argc, char* argv[]) {
  bool globalRunning = false;
  SDL_Window* window;
  SDL_GLContext gl_context;

#if KATO_INTERNAL
  void* memory_base_address = (void*)TERABYTES(2);
#else
  void* memory_base_address = (void*)0;
#endif
  kato::app::AppMemory app_memory = {};
  app_memory.permanent_storage_size = MEGABYTES(64);
  app_memory.transient_storage_size = GIGABYTES(1);

  app_memory.permanent_storage = mmap(
      memory_base_address,
      app_memory.permanent_storage_size + app_memory.transient_storage_size,
      PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  app_memory.transient_storage =
      (uint8*)app_memory.permanent_storage + app_memory.permanent_storage_size;

  // NOTE(phil)Initialize SDL2 with the default subsystems
  // and the subsystems specified as flags.
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0) {
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

    int sound_samples_per_second = 48000;
    // NOTE(phil): The number of samples in the sdl sound buffer is the number
    // of samples pulled from the SDL audio queue, however 1024 samples is the
    // minimum pulled (or perhaps 4096 bytes? The format could change making the
    // number of bytes per sample increase or decrease.). We need to keep this
    // number low enough so that the queue is never empty. i.e. The target
    // number of samples in the audio queue needs to be greater than or equal to
    // the number of samples in the sdl sound buffer, but we want to minimize
    // the size of the audio queue to have the lowest possible audio latency.
    // NOTE(phil): The sound buffer size in samples needs to be a power of 2.
    uint32 sound_buffer_size_in_samples = 1024;
    SDL_AudioDeviceID audio_device_id =
        SDLInitAudio(sound_samples_per_second, sound_buffer_size_in_samples);

    uint32 bytes_per_sound_sample = sizeof(int16) * 2;
    float target_sound_queue_frames = 2.f;
    uint32 target_sound_queue_samples =
        target_sound_queue_frames * (float)sound_samples_per_second / 60.f;
    uint32 target_sound_queue_bytes =
        target_sound_queue_samples * bytes_per_sound_sample;
    printf("Target sound queue frames: %f\n", target_sound_queue_frames);
    printf("Target sound queue samples: %u\n", target_sound_queue_samples);
    printf("Target sound queue bytes: %u\n", target_sound_queue_bytes);

    void* sound_buffer = malloc(target_sound_queue_bytes);
    kato::app::AppSoundOutput app_sound_output = {};
    app_sound_output.samples_per_second = sound_samples_per_second;
    app_sound_output.samples = (int16*)sound_buffer;
    app_sound_output.running_sample_index = 0;

    if (app_memory.permanent_storage && app_memory.transient_storage &&
        window && audio_device_id) {
      gl_context = SDL_GL_CreateContext(window);
      if (gl_context) {
        // NOTE(phil) Load all OpenGL functions using the SDL2 loader function
        if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
          globalRunning = true;

          kato::app::AppInput input[2] = {};
          kato::app::AppInput* new_input = &input[0];
          kato::app::AppInput* last_input = &input[1];

          uint64 begin_cycle_count = _rdtsc();
          uint64 perf_count_freq = SDL_GetPerformanceFrequency();
          uint64 begin_counter = SDL_GetPerformanceCounter();
          while (globalRunning) {
            // TODO(phil): For each button copy the pressed state. Zero the
            // transition counts somehow.
            // TODO(phil): Zero the new input?
            // TODO(phil): Zero macro?
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


            uint32 sound_bytes_to_write =
                target_sound_queue_bytes -
                SDL_GetQueuedAudioSize(audio_device_id);
            uint32 sound_samples_to_write =
                sound_bytes_to_write / bytes_per_sound_sample;
            printf("Sound samples to write: %u\n", sound_samples_to_write);
            printf("Sound bytes to write: %u\n", sound_bytes_to_write);

            app_sound_output.samples_to_write = sound_samples_to_write;
            kato::app::AppGetSoundSamples(&app_memory, &app_sound_output);

            SDL_QueueAudio(audio_device_id, sound_buffer, sound_bytes_to_write);

            kato::app::AppUpdateAndRender(&app_memory, new_input);
            // NOTE(phil): Render, swapping back and front buffer.
            // TODO(phil): Fix this call causing the framerate to be limited to
            // 60fps
            SDL_GL_SwapWindow(window);

            // TODO(phil): Determine if this is the correct place to end the
            // performace counting and cycle counting. Might make more sense to
            // end before flipping the frame?
            uint64 end_cycle_count = _rdtsc();
            uint64 elapsed_cycles = end_cycle_count - begin_cycle_count;
            double frame_million_cycles =
                ((double)elapsed_cycles / (1000.L * 1000.L));

            uint64 end_counter = SDL_GetPerformanceCounter();
            uint64 elapsed_counter = end_counter - begin_counter;
            double frame_ms =
                (1000.L * (double)elapsed_counter) / (double)perf_count_freq;
            double fps = (double)perf_count_freq / (double)elapsed_counter;

            printf("%.02f ms/f, %0.2ff/s, %.02fmc/f\n", frame_ms, fps,
                   frame_million_cycles);

            begin_cycle_count = end_cycle_count;
            begin_counter = end_counter;

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
