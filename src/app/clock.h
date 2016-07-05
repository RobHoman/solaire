// Copyright 2016 Phil Homan

#ifndef APP_CLOCK_H_
#define APP_CLOCK_H_

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
#endif  // APP_CLOCK_H_
