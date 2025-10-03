#pragma once

#include <vector>

#include "vec2.hpp"

// A disk particle
struct Particle {

  vec2r pos; // Position
  vec2r vel; // Velocity
  vec2r acc; // Acceleration

  double rot{0.0};  // Angular position
  double vrot{0.0}; // Angular velocity
  double arot{0.0}; // Angular acceleration

  double radius{0.0};
  double inertia{0.0};
  double mass{0.0};

  vec2r force; // resultant force
  double moment{0.0}; // resultant moment

  Particle(); // Ctor
};
