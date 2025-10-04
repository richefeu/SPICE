#include "pacOptionsManager.hpp"

void pacOptionsManager::process(SPICE & box) {
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