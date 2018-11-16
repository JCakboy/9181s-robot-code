#include "main.h"

PID::PID(int dt, double kp, double ki, double kd, int limit) {
  PID::dt = dt;
  PID::kp = kp;
  PID::ki = ki;
  PID::kd = kd;
  PID::limit = limit;
}

void PID::setdt(int value) {
  PID::dt = value;
}

void PID::setkp(double value) {
  PID::kp = value;
}

void PID::setki(double value) {
  PID::ki = value;
}

void PID::setkd(double value) {
  PID::kd = value;
}

void PID::setlimit(int value) {
  PID::limit = value;
}

void PID::modifydt(int delta) {
  PID::dt = PID::dt + delta;
}

void PID::modifykp(int delta) {
  PID::kp = PID::kp + delta;
}

void PID::modifyki(int delta) {
  PID::ki = PID::ki + delta;
}

void PID::modifykd(int delta) {
  PID::kd = PID::kd + delta;
}

void PID::modifylimit(int delta) {
  PID::limit = PID::limit + delta;
}

double PID::getdt() {
  return PID::dt;
}

double PID::getkp() {
  return PID::kp;
}

double PID::getki() {
  return PID::ki;
}

double PID::getkd() {
  return PID::kd;
}

double PID::getlimit() {
  return PID::limit;
}
