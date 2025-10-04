#pragma once

#include "SPICE.hpp"
#include "propertyProfile.hpp"

class propertiesManager{
public:
  bool hasDensity{false};
  bool hasFriction{false};
  bool hasRollingFriction{false};
  bool hasNormalStiffness{false};
  bool hasTangentialStiffness{false};
  bool hasAdhesion{false};
  bool hasGlueGc{false};
  
  propertyProfile<double> density;
  propertyProfile<double> friction;
  propertyProfile<double> stiffness;
  
  void process(SPICE & box);
};