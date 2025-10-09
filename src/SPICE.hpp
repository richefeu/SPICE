// ....
#pragma once

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>
#include <string>

#include "Interaction.hpp"
#include "Loading.hpp"
#include "Particle.hpp"
#include "farConnection.hpp"

#include "mat4.hpp"

#define SPICE_VERSION "2025.dev"
#define SPICE_WARN "\033[0m\033[31m\033[1m\033[4mWARN\033[24m\033[39m\033[0m: "
#define SPICE_INFO "\033[0m\033[32m\033[1m\033[4mINFO\033[24m\033[39m\033[0m: "

class SPICE {
public: 
  
  std::vector<Particle> Particles;
  std::vector<Interaction> Interactions;

  Loading *Load{nullptr};
  FarConnection bottom;
  FarConnection top;

  // parameters
  double t{0.0};
  double tmax{5.0};
  double dt{1e-6};
  
  double interLookC{0.0};
  double interCloseC{0.0}, interClose{0.01}, dVerlet{0.0};
  double interOutC{0.0}, interOut{0.1};
  double interHistC{0.0}, interHist{0.25};
  
  int iconf{0};
  vec2r gravity;
  mat4r Sig;
  mat4r SigConnect;

  size_t nBottom{0};
  size_t nTop{0};

  double xmin{0.0};
  double xmax{0.0};
  double ymin{0.0};
  double ymax{0.0};
  
  bool verbose{false};

  void head();

  // Core functions for the computations
  void integrate();
  void accelerations();
  void computeForcesAndMoments();
  void computeFarConnectionForces();
  void resetCloseList(double dmax);
  void combineParameters();

  // Save and Load the configuration files (conf-files)
  void saveConf(int i);
  void saveConf(const char *name);
  void loadConf(int i);
  void loadConf(const char *name);

  // Functions for updating relevant details
  void updateYrange(); // ymin and ymax
  void updateTotalMass(); // massTot
  void updateSizeRange(); // Rmin and Rmax

  void screenLog();
  
private:
  
  double getBranchShift(double xbranch, double Lperiod);
  
  // combination -> to be replaced by a lambda
  double harmonicMean(double x, double y);

public:  
  // pre-processing
  void capture(FarConnection &field, double hmin, double hmax);
  // TODO: re-set properties with profile
  // TODO: setTimeStep(double divisor);
};
