#include "main.h"

PID::PID(int dt, double kp, double ki, double kd, int limit) {
  PID::dt = dt;
  PID::kp = kp;
  PID::ki = ki;
  PID::kd = kd;
  PID::limit = limit;
}
