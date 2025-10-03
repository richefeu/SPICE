#include "Loading.hpp"
#include "SPICE.hpp"

// Remark: if the creation fails, nullptr is returned
Loading *Loading::create(const std::string &token) {
  if (token == "ShearPV") {
    return new ShearPV();
  } else if (token == "ShearVV") {
    return new ShearVV();
  }

  return nullptr;
}

Loading::~Loading() {}

// ========== ShearVV

ShearVV::ShearVV() {}

void ShearVV::read(std::istream &is) {
  is >> vx >> vy;
}

void ShearVV::write(std::ostream &os) {
  os << "ShearVV " << vx << ' ' << vy << std::endl;
}

void ShearVV::velocityVerlet_halfStep1(double /*dt*/) {}

void ShearVV::velocityVerlet_halfStep2(double /*dt*/) {}

void ShearVV::forceDrivenAcceleration() {}

// ========== ShearPV

ShearPV::ShearPV() {}

void ShearPV::read(std::istream &is) {
  is >> pressure >> velocity;
}

void ShearPV::write(std::ostream &os) {
  os << "ShearPV " << pressure << ' ' << velocity << std::endl;
}

void ShearPV::velocityVerlet_halfStep1(double /*dt*/) {}

void ShearPV::velocityVerlet_halfStep2(double /*dt*/) {}

void ShearPV::forceDrivenAcceleration() {}
