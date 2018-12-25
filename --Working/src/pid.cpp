#include "main.h"

PID::PID(int dt, double kp, double ki, double kd, bool brake, int tLimit, int iLimit, int dThreshold, int tThreshold, int de0){
  PID::dt = dt;
  PID::kp = kp;
  PID::ki = ki;
  PID::kd = kd;
  PID::brake = brake;
  PID::tLimit = tLimit;
  PID::iLimit = iLimit;
  PID::dThreshold = dThreshold;
  PID::tThreshold = tThreshold;
  PID::de0 = de0;
}

PIDCommand::PIDCommand(pid_command_type type, int result) {
  PIDCommand::type = type;
  PIDCommand::result = result;
}
