#include "pacOptionsManager.hpp"

void pacOptionsManager::process(SPICE &box) {
  if (!needProcess) { return; }

  box.verbose = verbose;
  
  if (inclination == 0.0) {
    box.gravity = gravity;
  } else {
    double g = norm(gravity);
    std::cout << SPICE_INFO << "Inclination of the gravity: " << inclination << " degrees (norm = " << g << ")"
              << std::endl;
    box.gravity.x = sin(inclination * M_PI / 180.0) * g;
    box.gravity.y = -cos(inclination * M_PI / 180.0) * g;
    std::cout << SPICE_INFO << "Results in gravity: " << box.gravity << std::endl;
  }

  box.t          = t;
  box.dt         = dt; // TODO: add possibility to set with portion of critical time step
  box.tmax       = tmax;
  box.interClose = interClose;
  box.interOut   = interOut;
  box.interHist  = interHist;
  box.dVerlet    = dVerlet;

  if (loading[0] != '!') {
    std::istringstream iss(loading);
    std::string token;
    iss >> token;

    box.Load = Loading::create(token.c_str());
    if (box.Load == nullptr) {
      std::cout << SPICE_WARN << "Could not create the Loading" << std::endl;
    } else {
      box.Load->box = &box; // not sure it is required
      box.Load->read(iss);
    }
  }
}