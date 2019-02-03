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

  // Set the aligner to null
  Puncher::useAligner = false;
  Puncher::driveControl = NULL;
  Puncher::vision = NULL;
  Puncher::strafeAlign = false;
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
    Puncher::motor->set_brake_mode(BRAKE_COAST);
    Puncher::motor->move(0);
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

void Puncher::setAligner(bool enabled, DriveControl * driveControl, pros::Vision * vision, bool strafeAlign) {
  if (enabled) {
    // Aligner requested to be disabled
    Puncher::useAligner = false;
    Puncher::driveControl = NULL;
    Puncher::vision = NULL;
    Puncher::strafeAlign = false;
    return;
  }
  // If either of the required object pointers are null, ignore this call
  if (driveControl == NULL || vision == NULL)
    return;
  // Set a new aligner
  Puncher::useAligner = enabled;
  Puncher::driveControl = driveControl;
  Puncher::vision = vision;
  Puncher::strafeAlign = strafeAlign;
}

void Puncher::align(bool pause) {
  // If the aligner is disabled, ignore this call
  if (!Puncher::useAligner) return;

  // Whether the puncher is aligned
  bool aligned = false;

  // Run at least one, if pausing is requested, continue until aligned
  do {
    // Read the vision signature
    pros::vision_object_s_t signature = vision->get_by_sig(0, (LCD::isAutonomousRed() ? redSigID : blueSigID));

    // Align
    if ((util::sign(signature.x_middle_coord) - visionMidXPos) < -5)
      driveControl->runStrafe(0, 35, strafeAlign, true, false, 1.0, 1.0);
    else if ((util::sign(signature.x_middle_coord) - visionMidXPos) > 5)
      driveControl->runStrafe(0, -35, strafeAlign, true, false, 1.0, 1.0);
    else {
      aligned = true;
      driveControl->stop(true);
      if (PUNCHER_VISION_ALIGNMENT_RUMBLE)
        ports::controllerMain->rumble(PUNCHER_VISION_ALIGNMENT_RUMBLE_PATTERN);
    } 

    // Delay for 20ms if looping
    if (pause) pros::delay(20);
  } while (pause && !aligned);
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

  if (std::lround(punchPos) % 360 > 350) punching = false;
}
