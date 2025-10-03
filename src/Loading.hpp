#pragma once

#include <iostream>

class SPICE;

// Mother class for any lodaing condition
class Loading {
public:
  SPICE *box;

  static Loading *create(const std::string &token);

  virtual void read(std::istream &is) = 0;
  virtual void write(std::ostream &os) = 0;
  virtual void servo() {}
  virtual void velocityVerlet_halfStep1(double dt) = 0;
  virtual void velocityVerlet_halfStep2(double dt) = 0;
  virtual void forceDrivenAcceleration() = 0;

  //Loading() = delete; // deactivated Ctor
  virtual ~Loading(); // virtual Dtor
};

class ShearVV : public Loading {
public:
  double vx;
  double vy;

  ShearVV();

  virtual void read(std::istream &is);
  virtual void write(std::ostream &os);
  virtual void velocityVerlet_halfStep1(double dt);
  virtual void velocityVerlet_halfStep2(double dt);
  virtual void forceDrivenAcceleration();
};

class ShearPV : public Loading {
public:
  double pressure;
  double velocity;

  ShearPV();

  virtual void read(std::istream &is);
  virtual void write(std::ostream &os);
  virtual void velocityVerlet_halfStep1(double dt);
  virtual void velocityVerlet_halfStep2(double dt);
  virtual void forceDrivenAcceleration();
};
