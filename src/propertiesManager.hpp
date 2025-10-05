#pragma once

#include <iostream>

#include "SPICE.hpp"
#include "propertyProfile.hpp"

class propertiesManager {
public:
  bool hasDensity{false};
  bool hasNormalStiffness{false};
  bool hasTangentialStiffness{false};
  bool hasNormalViscDampingRate{false};
  bool hasFriction{false};
  bool hasRollingFriction{false};
  bool hasAdhesion{false};
  bool hasGcGlue{false};

  propertyProfile<double> density;
  propertyProfile<double> normalStiffness;
  propertyProfile<double> tangentialStiffness;
  propertyProfile<double> normalViscDampingRate;
  propertyProfile<double> friction;
  propertyProfile<double> rollingFriction;
  propertyProfile<double> adhesion;
  propertyProfile<double> GcGlue;

  void process(SPICE &box);
};