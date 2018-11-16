#ifndef _PID_HPP_
#define _PID_HPP_

#include "main.h"

/*
 * A class meant to hold and manipulate robot-specific PID constants
 */

class PID {
  friend class DriveControl;
  private:
    int dt;
    double kp;
    double ki;
    double kd;
    int limit;

  public:
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

    // Sets dt to the given value
    void setdt(int value);

    // Sets kp to the given value
    void setkp(double value);

    // Sets ki to the given value
    void setki(double value);

    // Sets kd to the given value
    void setkd(double value);

    // Sets limit to the given value
    void setlimit(int value);

    // Adds the given value to dt
    void modifydt(int delta);

    // Adds the given value to kp
    void modifykp(int delta);

    // Adds the given value to ki
    void modifyki(int delta);

    // Adds the given value to kd
    void modifykd(int delta);

    // Adds the given value to limit
    void modifylimit(int delta);

    // Returns the set dt
    double getdt();

    // Returns the set kp
    double getkp();

    // Returns the set ki
    double getki();

    // Returns the set kd
    double getkd();

    // Returns the set limit
    double getlimit();
};

#endif
