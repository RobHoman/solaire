// Copyright 2016 Phil Homan

#ifndef CLOCK_H_
#define CLOCK_H_

class Clock {
 public:
  Clock();
  ~Clock();

  void Init();
  unsigned int GetTicks();
  unsigned int Restart();

 private:
  unsigned int start_sdl_ticks_;
};
#endif  // CLOCK_H_
