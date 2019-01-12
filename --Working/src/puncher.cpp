#include "main.h"

void Puncher::move(int degrees) {
  if (Puncher::lock->take(MUTEX_WAIT_TIME)) {
    // Update the motor target and issue the move absolute command
    Puncher::puncherTarget += degrees;
    Puncher::motor->move_absolute(Puncher::puncherTarget, 127);
    Puncher::lock->give();
  }
}

Puncher::Puncher(pros::Mutex * lock, pros::Motor * motor) {
  // Store the mutex
  Puncher::lock = lock;
  // Store the puncher motor
  Puncher::motor = motor;

  // Set the puncher target to 0
  Puncher::puncherTarget = 0;

  // Set the puncher flags to false
  Puncher::punching = false;
  Puncher::punchWaiting = false;
  Puncher::isprimed = false;
}

void Puncher::prime() {
  if (!Puncher::isprimed) {
    // Reset the puncher position and Flags, brake the motor, and pull it back
    Puncher::motor->tare_position();
    Puncher::motor->set_brake_mode(BRAKE_BRAKE);
    Puncher::puncherTarget = 0;
    Puncher::move(PUNCHER_PRIME_AMOUNT);
    Puncher::isprimed = true;
  }
}

void Puncher::unprime() {
  if (Puncher::isprimed) {
    // Unbrake the motor and restore its neutral position
    Puncher::isprimed = false;
  }
}

void Puncher::togglePrime() {
  // Toggle between prime states: if primed, unprime; if unprimed, prime
  if (Puncher::isprimed)
    Puncher::unprime();
  else
    Puncher::prime();
}

void Puncher::shoot() {
  // Sets the flag for an awaiting punch
  Puncher::punchWaiting = true;
}

bool Puncher::primed() {
  // Returns whether the puncher is primed
  return Puncher::isprimed;
}

void Puncher::run() {
  if (!Puncher::isprimed) {
    // If the puncher is not primed, ensure that the motor is in the neutral position
    Puncher::motor->set_brake_mode(BRAKE_COAST);
    if (!punching){
      Puncher::motor->tare_position();
      Puncher::motor->move(0);
    }

    Puncher::puncherTarget = 0;
  }

  if (Puncher::punchWaiting && !Puncher::punching) {
    // If a punch is waiting, punch
    Puncher::punchWaiting = false;
    Puncher::punching = true;
    Puncher::move(360);
  }

  // Check for punch completion
  int punchPos = Puncher::motor->get_position();
  if (Puncher::isprimed) punchPos -= PUNCHER_PRIME_AMOUNT;

  if (std::lround(punchPos) % 360 > 355) punching = false;
}
