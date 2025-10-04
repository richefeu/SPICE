#pragma once

#include <vector>

#include "vec2.hpp"

class FarConnection {
public:
  std::vector<size_t> Idx;
  
  //
  //     O
  //    / branch
  //   x pos
  //  
  std::vector<vec2r> pos; 
  // it initially corresponds to the particle positions
  // but these positions will be controlled for applying far field displacements (or...)
  // and a spring (stiffness K) acts between these points and the attached particles 
  
  double K{10000.0};
  
  // we will maybe add a viscosity also
  // double visc{0.0};
};
