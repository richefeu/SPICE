#pragma once

#include <string>

#include "SPICE.hpp"

class farConnectionManager {
public:
  std::string option{""};
  std::string location{"particles"};
  double thickness{0.1};
  double Young{1e8};
  double viscosity{0.0};
  
  void process(SPICE & box);
};