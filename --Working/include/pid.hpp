#ifndef _PID_HPP_
#define _PID_HPP_

#include "main.h"

/*
 * A class meant to hold and manipulate robot-specific PID constants
 */

class PID {
  public:
    int dt;
    double kp;
    double ki;
    double kd;
    bool brake;
    int tLimit;
    int iLimit;
    int iZone;
    bool iReset;
    int dThreshold;
    int tThreshold;
    int de0;

    /*
     * Creates the PID object
     *
     * dt (time delta): the minimum amount of time in ms between PID calculations, should be 20
     * kp (proportional gain): the constant for prorportional gain
     * ki (integral gain): the constant for integral gain
     * kd (derivitive gain): the constant for derivitive gain
     * brake: whether to brake when error is 0, allows for a more aggressive acceleration profile but more abrupt movement
     * tLimit (total limit): the maximum value for the sum of all PID terms; the maximum motor power, should be 127
     * iLimit (integral limit): the maximum value the integral term can be, before being multiplied to ki
     * iZone (integral zone): the error at which the integral term will contribute to the output
     * iReset (integral reset): whether to reset after the error is insignificant, should be true for movement and false for velocity
     * dThreshold (delta threshold): the maximum delta between the motors actual position and the desired position
     * tThreshold (time threshold): the minimum number of cycles the motor must hold within the delta threshold to consider the operation complete
     * de0 (hang threshold): the maximum number of cycles the motor holds a error delta of 0 before the operation is considered hung, use 0 to disable
     */
    explicit PID(int dt, double kp, double ki, double kd, bool brake, int tLimit, int iLimit, int iZone, bool iReset,  int dThreshold, int tThreshold, int de0);

};

/*
 * A class meant to hold calculation values for a PID controller
 * Used to preserve values between each calculation in different scopes
 */

class PIDCalc {
  public:
    int lastError;
    int Se;
    int hangCycles;
    int completeCycles;
};

/*
 * An enumeration specifying PID calculation statuses meant to used to communicate between the PID loop and the PID controller
 */
 typedef enum pid_command_type : int {
   E_COMMAND_NO_CALCULATION,
   E_COMMAND_CONTINUE,
   E_COMMAND_STOP,
   E_COMMAND_STOP_BRAKE,
   E_COMMAND_WAIT_SUCCESS,
   E_COMMAND_WAIT_HANG,
   E_COMMAND_EXIT_SUCCESS,
   E_COMMAND_EXIT_FAILURE
 } pid_command_type;

/*
 * A class meant to hold the result of a pid calculation
 */
class PIDCommand {
  public:
    // The command type
    pid_command_type type;
    // The calculation result meant to be sent to the motor
    int result;

    /*
     * Creates the PID Command object
     *
     * type: the command type
     * result: the calculation result
     */
    PIDCommand(pid_command_type type, int result);

};

#endif
