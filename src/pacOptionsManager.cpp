#include "pacOptionsManager.hpp"

void pacOptionsManager::process(SPICE &box) {
  if (!needProcess) { return; }

  box.verbose    = verbose;
  box.gravity    = gravity;
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