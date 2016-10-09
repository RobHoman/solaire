// Copyright 2016 Phil Homan

#include <stdio.h>

#include "app/app.h"

#include "glad/glad.h"

#include "common/macros.h"
#include "kato/linux_platform.h"

namespace kato {
namespace app {

GLuint GLCreateVertexShader(const char* filepath) {
  GLuint shader_handle = glCreateShader(GL_VERTEX_SHADER);
  DebugReadFile shader_file = DebugPlatformReadEntireFile(filepath);
  const char* shader_text = (char*)shader_file.memory;
  glShaderSource(shader_handle, 1, &shader_text, nullptr);
  glCompileShader(shader_handle);

  return shader_handle;
}

GLuint GLCreateFragmentShader(const char* filepath) {
  GLuint shader_handle = glCreateShader(GL_FRAGMENT_SHADER);
  DebugReadFile shader_file = DebugPlatformReadEntireFile(filepath);
  const char* shader_text = (char*)shader_file.memory;
  glShaderSource(shader_handle, 1, &shader_text, nullptr);
  glCompileShader(shader_handle);

  return shader_handle;
}

void GLPrintShaderCompileError(GLuint shader_handle) {
  GLint compile_success = 0;
  glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &compile_success);
  if (compile_success == GL_FALSE) {
    GLint max_log_length = 0;
    glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &max_log_length);

    char log_msg[max_log_length];
    glGetShaderInfoLog(shader_handle, max_log_length, &max_log_length, &log_msg[0]);

    printf("Shader error: %s\n", log_msg);
  }
}

GLuint GLCreateShaderProgram(const char* vertex_shader_filename,
                           const char* fragment_shader_filename) {
  GLuint vertex_shader_handle = GLCreateVertexShader(vertex_shader_filename);
  GLPrintShaderCompileError(vertex_shader_handle);

  GLuint fragment_shader_handle =
      GLCreateFragmentShader(fragment_shader_filename);
  GLPrintShaderCompileError(fragment_shader_handle);

  GLuint shader_program_handle = glCreateProgram();
  glAttachShader(shader_program_handle, vertex_shader_handle);
  glAttachShader(shader_program_handle, fragment_shader_handle);

  glLinkProgram(shader_program_handle);
  // TODO(phil): Should probably remove the use program here when this is
  // tailored for generic use.
  glUseProgram(shader_program_handle);

  return shader_program_handle;
}

// TODO(phil): Make stuff in here generic when appropriate
void GLInitVertexBuffer(GLuint shader_program_handle) {
  float fullscreen_vertices[8] = {-1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0};

  GLuint vertex_array_object;
  glGenVertexArrays(1, &vertex_array_object);
  glBindVertexArray(vertex_array_object);

  GLuint vertex_buffer_object;
  glGenBuffers(1, &vertex_buffer_object);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(fullscreen_vertices),
               fullscreen_vertices, GL_STATIC_DRAW);

  GLuint vertex_position_attrib_location =
      glGetAttribLocation(shader_program_handle, "vertex_position");

  glEnableVertexAttribArray(vertex_position_attrib_location);
  glVertexAttribPointer(vertex_position_attrib_location, 2, GL_FLOAT, GL_FALSE,
                        0, 0);

  // TODO(phil): Unbind things to reset OpenGl state.
}

void AppUpdateAndRender(AppMemory* memory, AppInput* input) {
  ASSERT(sizeof(AppState) <= memory->permanent_storage_size);
  AppState* app_state = (AppState*)memory->permanent_storage;

  if (!memory->is_initialized) {
    // TODO(phil): Remove reading and writing a file once these functions are
    // used elsewhere. Keeping them for now in order to always make sure the
    // functions are "tested".
    DebugReadFile appFile = DebugPlatformReadEntireFile("rules.ninja");
    DebugPlatformWriteEntireFile("test.out", appFile.size, appFile.memory);
    DebugPlatformFreeFileMemory(appFile.size, appFile.memory);

    app_state->tone_hz = 256.f;
    app_state->tone_volume = 3000;

    GLuint shader_program_handle =
        GLCreateShaderProgram("vshaderTest.glsl", "fshaderTest.glsl");
    GLInitVertexBuffer(shader_program_handle);

    memory->is_initialized = true;
  }

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glViewport(0.0, 0.0, 1280, 1020);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void AppOutputSound(AppSoundOutput* app_sound_output, float tone_hz,
                    int16 tone_volume) {
  int samples_per_second = app_sound_output->samples_per_second;

  int square_wave_period = samples_per_second / tone_hz;
  int half_square_wave_period = square_wave_period / 2;

  int16* sound_sample = app_sound_output->samples;
  uint32 sample_count = app_sound_output->samples_to_write;
  for (int sample_index = 0; sample_index < sample_count; ++sample_index) {
    int16 sample_value =
        ((app_sound_output->running_sample_index++ / half_square_wave_period) %
         2)
            ? tone_volume
            : -tone_volume;
    *sound_sample++ = sample_value;
    *sound_sample++ = sample_value;
  }
}

void AppGetSoundSamples(AppMemory* memory, AppSoundOutput* app_sound_output) {
  ASSERT(sizeof(AppState) <= memory->permanent_storage_size);
  AppState* app_state = (AppState*)memory->permanent_storage;
  AppOutputSound(app_sound_output, app_state->tone_hz, app_state->tone_volume);
}

}  // namespace app
}  // namespace kato
