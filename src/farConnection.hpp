#pragma once

#include <vector>

#include "vec2.hpp"

// Maybe we should rename it Connection or SoftConnection ??????
class FarConnection {
public:
  std::vector<size_t> Idx;
  
  //
  //     O
  //    / l0
  //   x pos
  //  
  std::vector<vec2r> pos; 
  // it initially corresponds to the particle positions
  // but these positions will be controlled for applying far field displacements (or...)
  // and a spring (stiffness K) acts between these points and the attached particles 
  
  double K{10000.0};
  
  // we will maybe add a viscosity also
  // double visc{0.0};
  
  // sauvegarde de forces dans les ressorts
  //std::vector<double> l0;
  //std::vector<double> l;
  //std::vector<vec2r> n;
  //std::vector<vec2r> force;
  
  // Pour controler en force ou pression il faudrait peut-être avoir une mass et une force résultante
  // double mass
  // vec2r resultantForce;
  
};
