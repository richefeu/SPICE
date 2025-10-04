#pragma once

#include <string>
#include "SPICE.hpp"

class pacOptionsManager {
public:
  std::string generatedFileName{"input.txt"};
  std::string loading{"!!! TO BE DEFINED"};
  bool verbose{true};
  
  
  void process(SPICE & box);
};