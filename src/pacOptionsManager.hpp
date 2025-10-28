#pragma once

#include <string>
#include "SPICE.hpp"

class pacOptionsManager {
public:
  std::string generatedFileName{"input.txt"};
  std::string loading{"!!! TO BE DEFINED"};
  bool verbose{true};
  vec2r gravity{0, 9.81};
  double inclination{0.0}; 
  double t{0.0};
  double tmax{1.0};
  double dt{1e-6};
  double interClose{10e-6};
  double interOut{0.1};
  double interHist{0.25};
  double dVerlet{1.0};
  
  void process(SPICE & box);
  bool needProcess{false};
};