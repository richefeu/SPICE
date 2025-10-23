#pragma once

#include <vector>

#include "SPICE.hpp"
#include "propertyProfile.hpp"

class packingManager {
public:
  std::string option{"nothing-to-do"};
  size_t nx{10};
  size_t ny{10};
  propertyProfile<double> radius;
  propertyProfile<double> radiusVariation;
  double density{2700.0};
  bool includeFarConnection{true};
  bool hasBottomLine{false};
  bool hasTopLine{false};
  int bottomNumber{0};
  int topNumber{0};
  
  void process(SPICE & box);
  bool needProcess{false};
  
private:
  void grid(SPICE & box);
  void geoDeposite(SPICE & box);
  
};