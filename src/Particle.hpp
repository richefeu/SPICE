#pragma once

#include <vector>

#include "toofus/vec2.hpp"

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

  vec2r force;        // resultant force
  double moment{0.0}; // resultant moment

  // Embeded parameters
  double normalStiffness{1e6};
  double tangentialStiffness{1e6};
  double normalViscDampingRate{0.98};
  double friction{0.0};
  double rollingFriction{0.0};
  double adhesion{0.0};
  double GcGlue{0.0};

  Particle(); // Ctor
};
