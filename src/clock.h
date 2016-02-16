// Copyright 2016 Phil Homan

#ifndef SRC_CLOCK_H_
#define SRC_CLOCK_H_

class Clock {
 public:
  Clock();
  ~Clock();

  void Init();
  double GetSeconds();
  double Restart();

 private:
  double start_glfw_time_;
};
#endif  // SRC_CLOCK_H_
