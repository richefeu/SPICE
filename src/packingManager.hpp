#pragma once

#include <vector>

#include "SPICE.hpp"
#include "propertyProfile.hpp"

class packingManager {
public:
  std::string option{"grid"};
  size_t nx{10};
  size_t ny{10};
  propertyProfile<double> radius;
  propertyProfile<double> radiusVariation;
  double density{2700.0};
  bool includeFarConnection{true};
  
  
  void process(SPICE & box);
  
private:
  void grid(SPICE & box);
  void geoDeposite(SPICE & box);
  
};