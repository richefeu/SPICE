#include "packingManager.hpp"

void packingManager::process(SPICE &box) {
  if (!needProcess) { return; }

  if (option == "nothing-to-do") {
    return;
  } else if (option == "grid") {
    grid(box);
  } else if (option == "geo-deposite") {
    geoDeposite(box);
  } else {
    std::cout << "Unknown packing option: " << option << std::endl;
  }
}

// TODO: addedGap (it could be usefull)
//  initial pertuabation (displacement within addedGap)
//  initial perturbation (velocities)
void packingManager::grid(SPICE &box) {

  Particle P;
  P.rot = 0.0;

  int ngw = (int)nx;
  int ngh = (int)ny;
  std::cout << "nx = " << ngw << std::endl;
  std::cout << "ny = " << ngh << std::endl;

  double rvarMax = radiusVariation.getMaxValue();
  double rmax    = radius.getMaxValue() + rvarMax; // + addedGap
  double rmin    = radius.getMinValue();

  std::cout << "Maximum radius = " << rmax << std::endl;
  std::cout << "Maximum radius variation = " << rvarMax << std::endl;
  vec2r from(0.0, 0.0);
  // vec2r to(2.0 * (ngw + 1) * rmax, 2.0 * (ngh + 1) * rmax);
  vec2r to(2.0 * ngw * rmax + rmax, 2.0 * (ngh + 1) * rmax);
  box.dVerlet = 0.95 * rmin;

  if (hasBottomLine) {
    P.radius = 0.5 * to.x / (double)bottomNumber;
    P.pos.y  = -P.radius;
    double x = P.radius;
    while (x < to.x) {
      P.pos.x = x;
      box.Particles.push_back(P);
      x += 2.0 * P.radius;
    }
  }

  size_t idFirst        = box.Particles.size();
  int k                 = 0;
  double step           = to.x / (2.0 * ngw);
  double triangleFactor = 0.5 * sqrt(3.0);
  while (P.pos.y < to.y) {
    int column = k % ngw;
    int row    = k / ngw;
    if (row % 2 == 0) { // even row
      P.pos.x = step + 2 * column * step;
    } else { // odd row
      P.pos.x = 2 * step + 2 * column * step;
    }
    P.pos.y = triangleFactor * step * (1 + 2 * row);
    if (P.pos.y <= to.y - step) {
      P.radius = rmax; // temporary radius
      box.Particles.push_back(P);
    }
    k++;
  }
  size_t idLast = box.Particles.size();

  if (hasTopLine) {
    P.radius = 0.5 * to.x / (double)topNumber;
    P.pos.y  = box.Particles[idLast - 1].pos.y + rmax + P.radius;
    double x = P.radius;
    while (x < to.x) {
      P.pos.x = x;
      box.Particles.push_back(P);
      x += 2.0 * P.radius;
    }
  }

  // period in x direction
  box.xmin = from.x;
  box.xmax = to.x;
  box.updateYrange();
  from.y = box.ymin;
  to.y   = box.ymax;

  // recompute the radius, mass properties
  for (size_t i = idFirst; i < idLast; i++) {
    double relativeHeight = (box.Particles[i].pos.y - box.ymin) / (box.ymax - box.ymin);
    double R              = radius.getValueAt(relativeHeight);
    double Rvar =
        radiusVariation.getValueAt(relativeHeight) * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5);
    R += Rvar;
    box.Particles[i].radius = R;
    box.Particles[i].mass   = M_PI * R * R * density;
    box.Particles[i].inertia = 0.5 * box.Particles[i].mass * R * R;
  }

  // Far field attached particles
  if (includeConnections) {
    box.bottom.Idx.clear();
    box.bottom.pos.clear();
    box.top.Idx.clear();
    box.top.pos.clear();

    double bottomThickness = 2.0 * rmax;
    if (hasBottomLine) {
      bottomThickness = to.x / (double)bottomNumber;

      for (size_t i = 0; i < box.Particles.size(); i++) {
        if (box.Particles[i].pos.y < from.y + bottomThickness) {
          box.bottom.Idx.push_back(i);
          box.bottom.pos.push_back(box.Particles[i].pos);
        }
      }
    }

    double topThickness = 2.0 * rmax;
    if (hasTopLine) {
      topThickness = to.x / (double)topNumber;

      for (size_t i = 0; i < box.Particles.size(); i++) {
        if (box.Particles[i].pos.y > to.y - topThickness) {
          box.top.Idx.push_back(i);
          box.top.pos.push_back(box.Particles[i].pos);
        }
      }
    }
  }
}

// FIXME: not sure we will implement it
void packingManager::geoDeposite(SPICE & /*box*/) {}
