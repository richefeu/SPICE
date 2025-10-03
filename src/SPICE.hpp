// ....
#pragma once

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>

#include "FarField.hpp"
#include "Interaction.hpp"
#include "Loading.hpp"
#include "Particle.hpp"

#include "mat4.hpp"

#define SPICE_WARN "\033[0m\033[31m\033[1m\033[4mWARN\033[24m\033[39m\033[0m: "
#define SPICE_INFO "\033[0m\033[32m\033[1m\033[4mINFO\033[24m\033[39m\033[0m: "

class SPICE {
public:
  std::vector<Particle> Particles;
  std::vector<Interaction> Interactions;

  Loading *Load{nullptr};
  FarField bottom;
  FarField top;

  // parameters
  double t{0.0};
  double tmax{5.0};
  double dt{1e-6};
  double interLookC{0.0}, interLook{0.25};
  double interCloseC{0.0}, interClose{0.01}, dVerlet{0.0};
  double interOutC{0.0}, interOut{0.1};
  double interHistC{0.0}, interHist{0.25};
  double density{2700.0};
  double kn{1e4};
  double kt{1e4};
  double dampRate{0.95};
  double mu{0.8};
  int nstor{0};
  int iconf{0};
  vec2r gravity;
  mat4r Sig;

  size_t nBottom{0};
  size_t nTop{0};

  double xmin{0.0};
  double xmax{0.0};
  double ymin{0.0};
  double ymax{0.0};

  void funnyHead();
  void buildSampleInteractively();

  void integrate();
  void accelerations();
  void resetCloseList(double dmax);

  void saveConf(int i);
  void saveConf(const char *name);
  void loadConf(int i);
  void loadConf(const char *name);

private:
  void screenLog();
  double getBranchShift(double xbranch, double Lperiod);
  void updateYrange();
  void capture(FarField & field, double hmin, double hmax);
};
