#pragma once

#include "vec2.hpp"

struct Interaction {
  size_t i{0};
  size_t j{0};
  double fn{0.0};
  double ft{0.0};
  double damp{0.0};

  Interaction();
  Interaction(size_t I, size_t J, double Damp);
};
