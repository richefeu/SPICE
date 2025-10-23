#include "farConnectionManager.hpp"

void farConnectionManager::process(SPICE &box) {
  if (!needProcess) { return; }

  box.bottom.K = stiffness;
  box.top.K    = stiffness;
}