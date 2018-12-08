#ifndef _PID_HPP_
#define _PID_HPP_

#include "main.h"

/*
 * A class meant to hold and manipulate robot-specific PID constants
 */

class PID {
  friend class DriveControl;
  public:
    int dt;
    double kp;
    double ki;
    double kd;
    int limit;

    int lastError;
    int Se;

    /*
     * Creates the PID object
     *
     * dt (time delta): the minimum amount of time in ms between PID calculations
     * kp (proportional gain): the constant for prorportional gain
     * ki (integral gain): the constant for integral gain
     * kd (derivitive gain): the constant for derivitive gain
     * limit (integral limit): the maximum value the integral term can contribute to the final output power
     */
    explicit PID(int dt, double kp, double ki, double kd, int limit);

};

#endif
