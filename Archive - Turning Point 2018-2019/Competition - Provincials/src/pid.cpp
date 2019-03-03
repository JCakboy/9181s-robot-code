#include "main.h"

PID::PID(int dt, double kp, double ki, double kd, bool brake, int tLimit, double aLimit, int iLimit, int iZone, bool iReset, int dThreshold, int tThreshold, int de0){
  // Store all values
  PID::dt = dt;
  PID::kp = kp;
  PID::ki = ki;
  PID::kd = kd;
  PID::brake = brake;
  PID::tLimit = tLimit;
  PID::aLimit = aLimit;
  PID::iLimit = iLimit;
  PID::iZone = iZone;
  PID::iReset = iReset;
  PID::dThreshold = dThreshold;
  PID::tThreshold = tThreshold;
  PID::de0 = de0;
}

PIDCommand PID::calculate(PIDCalc * calc, int position, int target) {
  // Calculate the current error
  double error = target - position;

  if (PID::iReset && (error == 0 || (calc->Se > 0 && error < 0) || (calc->Se < 0 && error > 0))) calc->Se = 0; // Check for error sum relevancy
  else if (util::withinX(PID::iZone, error)) calc->Se = util::limitX(PID::iLimit, calc->Se + error); // Add the current error to the sum of all errors

  double de = (error - calc->lastError) / PID::dt; // Calculate the change in error over time (gradient)

  // Check for successful completion of the PID movement
  if (util::abs(error) <= PID::dThreshold) {
    calc->completeCycles++;
    if (calc->completeCycles >= PID::tThreshold)
      // Time has exceeded the tThreshold
      return PIDCommand(E_COMMAND_EXIT_SUCCESS, 0);
  } else
    calc->completeCycles = 0;

  // Check for the motor being stuck on something
  if (util::abs(de) <= 1 && util::abs(error) >= PID::dThreshold) {
    calc->hangCycles++;
    if (PID::de0 > 0 && calc->hangCycles >= PID::de0)
      // Time has exceeded the hangThreshold
      return PIDCommand(E_COMMAND_EXIT_FAILURE, 0);
  } else
    calc->hangCycles = 0;

  LCD::setText(2, "Error (T" + std::to_string(PID::dThreshold) + "): " + std::to_string(error));

  // Calculate the output of each term
  double p = error * PID::kp;
  double i = util::limitX(PID::iLimit, calc->Se * PID::ki);
  double d = de * PID::kd;

  double power = util::limitX(PID::tLimit, p + i + d);

  // Account for max acceleration
  if (util::abs(power) - util::abs(calc->lastPower) > PID::aLimit) {
    if (power > 0.0)
      power = calc->lastPower + PID::aLimit;
    if (power < 0.0)
      power = calc->lastPower - PID::aLimit;
    calc->hangCycles = 0;
  }

  long rpower = std::lround(power);
  LCD::setText(3, "Power " + std::to_string(rpower));

  // Store the error in the calculation object's lastError
  calc->lastError = error;
  // Store the power in the calculation object's lastPower
  calc->lastPower = power;

  // Return the PID command to the caller
  if (rpower == 0)
    if (PID::brake)
      return PIDCommand(E_COMMAND_STOP_BRAKE, 0);
    else return PIDCommand(E_COMMAND_STOP, 0);
  else
    return PIDCommand(E_COMMAND_CONTINUE, rpower);
}

PIDCommand::PIDCommand(pid_command_type type, int result) {
  // Store the command type and result
  PIDCommand::type = type;
  PIDCommand::result = result;
}
