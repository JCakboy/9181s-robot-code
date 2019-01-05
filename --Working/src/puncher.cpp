#include "main.h"

void Puncher::move(int degrees) {
  if (Puncher::lock->take(MUTEX_WAIT_TIME)) {
    Puncher::puncherTarget += degrees;
    Puncher::motor->move_absolute(Puncher::puncherTarget, 127);
    Puncher::lock->give();
  }
}

Puncher::Puncher(pros::Mutex * lock, pros::Motor * motor) {
  Puncher::lock = lock;
  Puncher::motor = motor;

  Puncher::puncherTarget = 0;

  Puncher::punching = false;
  Puncher::punchWaiting = false;
  Puncher::isprimed = false;
}

void Puncher::prime() {
  if (!Puncher::isprimed) {
    Puncher::motor->tare_position();
    Puncher::motor->set_brake_mode(BRAKE_BRAKE);
    Puncher::puncherTarget = 0;
    Puncher::move(PUNCHER_PRIME_AMOUNT);
    Puncher::isprimed = true;
  }
}

void Puncher::unprime() {
  if (Puncher::isprimed) {
    Puncher::isprimed = false;
  }
}

void Puncher::togglePrime() {
  if (Puncher::isprimed)
    Puncher::unprime();
  else
    Puncher::prime();
}

void Puncher::shoot() {
  Puncher::punchWaiting = true;
}

bool Puncher::primed() {
  return Puncher::isprimed;
}

void Puncher::run() {
  if (!Puncher::isprimed) {
    Puncher::motor->set_brake_mode(BRAKE_COAST);
    Puncher::motor->tare_position();
    Puncher::motor->move(0);

    Puncher::puncherTarget = 0;
  }

  if (Puncher::punchWaiting && !Puncher::punching) {
    Puncher::punchWaiting = false;
    Puncher::punching = true;
    Puncher::move(360);
  }

  int punchPos = Puncher::motor->get_position();
  if (Puncher::isprimed) punchPos -= PUNCHER_PRIME_AMOUNT;

  if (std::lround(punchPos) % 360 > 345) punching = false;
}
