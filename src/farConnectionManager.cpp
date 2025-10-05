#include "farConnectionManager.hpp"

void farConnectionManager::process(SPICE &box) {

  box.bottom.K = stiffness;
  box.top.K    = stiffness;
}