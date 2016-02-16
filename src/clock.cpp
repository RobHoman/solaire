// Copyright 2016 Phil Homan

#include <GLFW/glfw3.h>

#include "./clock.h"

Clock::Clock() {
  start_glfw_time_ = 0.0;
}

Clock::~Clock() {}

void Clock::Init() {
  start_glfw_time_ = glfwGetTime();
}

double Clock::GetSeconds() {
  return glfwGetTime() - start_glfw_time_;
}

double Clock::Restart() {
  double new_start_time = glfwGetTime();
  double seconds_at_restart = new_start_time - start_glfw_time_;
  start_glfw_time_ = new_start_time;
  return seconds_at_restart;
}
