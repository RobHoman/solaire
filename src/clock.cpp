// Copyright 2016 Phil Homan

#include "./clock.h"

#include "SDL/SDL_timer.h"

Clock::Clock() {
  start_sdl_ticks_ = 0;
}

Clock::~Clock() {}

void Clock::Init() {
  start_sdl_ticks_ = SDL_GetTicks();
}

unsigned int Clock::GetTicks() {
  return SDL_GetTicks() - start_sdl_ticks_;
}

unsigned int Clock::Restart() {
  unsigned int new_start_ticks = SDL_GetTicks();
  unsigned int ticks_at_restart = new_start_ticks - start_sdl_ticks_;
  start_sdl_ticks_ = new_start_ticks;
  return ticks_at_restart;
}
