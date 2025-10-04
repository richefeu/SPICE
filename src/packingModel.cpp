#include "packingModel.hpp"

void packingModel::pack(SPICE &box) {
  if (name == "grid") {
    grid(box);
  } else if (name == "geo-deposite") {
    geoDeposite(box);
  } else {
    std::cout << "Unknown model name" << std::endl;
  }
}

void packingModel::grid(SPICE &box) {

  Particle P;
  P.rot = 0.0;

  int ngw = (int)nx;
  int ngh = (int)ny;
  std::cout << "nx =  " << ngw << std::endl;
  std::cout << "ny =  " << ngh << std::endl;

  double rvarMax = radiusVariation.getMaxValue();
  double rmax    = radius.getMaxValue() + rvarMax;
  double rmin    = radius.getMinValue();
  // double deltaR = rmax - rmin;
  std::cout << "Maximum radius = " << rmax << std::endl;
  std::cout << "Maximum radius variation = " << rvarMax << std::endl;
  vec2r from(0.0, 0.0);
  vec2r to(2.0 * (ngw + 1) * rmax, 2.0 * (ngh + 1) * rmax);
  box.dVerlet = 0.95 * rmin;

  int i          = 0;
  double massTot = 0.0;
  double step    = to.x / (2.0 * ngw);
  while (P.pos.y < to.y) {
    int column = i % ngw;
    int row    = i / ngw;
    if (row % 2 == 0) { // even row
      P.pos.x = step + 2 * column * step;
    } else { // odd row
      P.pos.x = 2 * step + 2 * column * step;
    }
    P.pos.y = step + 2 * row * step;
    if (P.pos.y <= to.y - step) {
      P.radius = rmax; // temporary radius
      box.Particles.push_back(P);
    }
    i++;
  }

  // recompute bounding box
  from.set(1e20, 1e20);
  to.set(-1e20, -1e20);
  vec2r diag(1.0, 1.0);
  for (size_t i = 0; i < box.Particles.size(); i++) {
    vec2r pmin = box.Particles[i].pos - rmax * diag;
    vec2r pmax = box.Particles[i].pos + rmax * diag;
    if (from.x > pmin.x) { from.x = pmin.x; }
    if (from.y > pmin.y) { from.y = pmin.y; }
    if (to.x < pmax.x) { to.x = pmax.x; }
    if (to.y < pmax.y) { to.y = pmax.y; }
  }
  // period in x direction
  box.xmin = from.x;
  box.xmax = to.x;
  box.ymin = from.y;
  box.ymax = to.y;

  // recompute the radius, mass properties
  for (size_t i = 0; i < box.Particles.size(); i++) {
    double relativeHeight = (box.Particles[i].pos.y - box.ymin) / (box.ymax - box.ymin);
    double R              = radius.getValueAt(relativeHeight);
    double Rvar =
        radiusVariation.getValueAt(relativeHeight) * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5);
    R += Rvar;
    box.Particles[i].radius = R;
    box.Particles[i].mass   = M_PI * R * R * density;
    massTot += box.Particles[i].mass;
    box.Particles[i].inertia = 0.5 * box.Particles[i].mass * R * R;
  }

  // Far field attached particles
  if (includeFarConnection) {
    box.bottom.Idx.clear();
    box.bottom.pos.clear();
    box.bottom.K = 1000;
    box.top.Idx.clear();
    box.top.pos.clear();
    box.top.K = 1000;
    for (size_t i = 0; i < box.Particles.size(); i++) {
      if (box.Particles[i].pos.y < from.y + 2.0 * rmax) {
        box.bottom.Idx.push_back(i);
        box.bottom.pos.push_back(box.Particles[i].pos);
      } else if (box.Particles[i].pos.y > to.y - 2.0 * rmax) {
        box.top.Idx.push_back(i);
        box.top.pos.push_back(box.Particles[i].pos);
      }
    }
  }

  // Set ramdom velocities to particles
  /*
  double vmax = 0.1;
  std::cout << "Norm of ramdom velocities? ";
  std::cin >> vmax;
  for (size_t i = 0; i < Particles.size(); i++) {
    Particles[i].vel.x = vmax * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    Particles[i].vel.y = vmax * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
  }
  */
}

void packingModel::geoDeposite(SPICE & /*box*/) {}
