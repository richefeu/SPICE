// ....

#include "SPICE.hpp"

// ---------------------------------------------------------
// ---------------------------------------------------------
void SPICE::funnyHead() {
  std::cout << '\n';
  std::cout << "    zzzzzzzz" << '\n';
  std::cout << " >  oooooooo >    SPICE - 2025" << '\n';
  std::cout << " > oooooooo  >    Laboratoire 3SR" << '\n';
  std::cout << " >  OOOOOOOO >    Université Grenoble Alpes" << '\n';
  std::cout << " > OOOOOOOO  >    Complain to: vincent.richefeu@univ-grenoble-alpes.fr" << '\n';
  std::cout << "   zzzzzzzz                    cyrille.couture@univ-grenoble-alpes.fr" << '\n';
  std::cout << std::endl;
}

// ---------------------------------------------------------
// ---------------------------------------------------------
void SPICE::saveConf(int i) {
  char fname[256];
  sprintf(fname, "conf%d", i);
  saveConf(fname);
}

// ---------------------------------------------------------
// ---------------------------------------------------------
void SPICE::saveConf(const char *name) {
  std::ofstream conf(name);

  conf << "SPICE 2025" << std::endl; // format: progName version-date
  conf << std::setprecision(6);
  conf << "t " << t << std::endl;
  conf << "tmax " << tmax << std::endl;
  conf << "dt " << dt << std::endl;
  conf << "interLook " << interLook << std::endl;
  conf << "interClose " << interClose << std::endl;
  conf << "interOut " << interOut << std::endl;
  conf << "interHist " << interHist << std::endl;
  conf << "dVerlet " << dVerlet << std::endl;
  conf << "gravity " << gravity << std::endl;
  conf << "density " << density << std::endl;
  conf << "kn " << kn << std::endl;
  conf << "kt " << kt << std::endl;
  conf << "dampRate " << dampRate << std::endl;
  conf << "mu " << mu << std::endl;
  conf << "nstor " << nstor << std::endl;
  conf << "iconf " << iconf << std::endl;
  conf << "Xperiod " << xmin << " " << xmax << std::endl;
  conf << "bottom " << bottom.Idx.size() << " " << bottom.K << std::endl;
  conf << std::setprecision(15);
  for (size_t i = 0; i < bottom.Idx.size(); i++) { conf << bottom.Idx[i] << " " << bottom.pos[i] << std::endl; }
  conf << std::setprecision(6);
  conf << "top " << top.Idx.size() << " " << top.K << std::endl;
  conf << std::setprecision(15);
  for (size_t i = 0; i < top.Idx.size(); i++) { conf << top.Idx[i] << " " << top.pos[i] << std::endl; }
  conf << "Loading ";
  if (Load != nullptr) {
    Load->write(conf);
  } else {
    std::cout << SPICE_WARN << "Loading is not defined" << std::endl;
  }

  conf << "Particles " << Particles.size() << std::endl;
  for (size_t i = 0; i < Particles.size(); i++) {
    conf << Particles[i].pos << " " << Particles[i].vel << " " << Particles[i].acc << " " << Particles[i].rot << " "
         << Particles[i].vrot << " " << Particles[i].arot << " " << Particles[i].radius << " " << Particles[i].inertia
         << " " << Particles[i].mass << std::endl;
  }
  conf << "Interactions " << Interactions.size() << std::endl;
  for (size_t i = 0; i < Interactions.size(); i++) {
    if (fabs(Interactions[i].fn) < 1.0e-20) { continue; }
    conf << Interactions[i].i << " " << Interactions[i].j << " " << Interactions[i].fn << " " << Interactions[i].ft
         << " " << Interactions[i].damp << std::endl;
  }
}

// ---------------------------------------------------------
// ---------------------------------------------------------
void SPICE::loadConf(int i) {
  char fname[256];
  sprintf(fname, "conf%d", i);
  loadConf(fname);
}

// ---------------------------------------------------------
// ---------------------------------------------------------
void SPICE::loadConf(const char *name) {
  std::ifstream conf(name);
  if (!conf.is_open()) { std::cout << SPICE_WARN << "Cannot read " << name << std::endl; }

  // Check header
  std::string prog;
  conf >> prog;
  if (prog != "SPICE") { std::cout << SPICE_WARN << "This is not file for SPICE executable!" << std::endl; }
  std::string date;
  conf >> date;
  if (date != "2025") { std::cout << SPICE_WARN << "The version-date should be 2025!" << std::endl; }

  std::string token;
  conf >> token;
  while (conf.good()) {
    if (token[0] == '/' || token[0] == '#' || token[0] == '!') {
      getline(conf, token); // ignore the rest of the current line
      conf >> token;        // next token
      continue;
    } else if (token == "t") {
      conf >> t;
    } else if (token == "tmax") {
      conf >> tmax;
    } else if (token == "dt") {
      conf >> dt;
    } else if (token == "interLook") {
      conf >> interLook;
    } else if (token == "interClose") {
      conf >> interClose;
    } else if (token == "interOut") {
      conf >> interOut;
    } else if (token == "interHist") {
      conf >> interHist;
    } else if (token == "dVerlet") {
      conf >> dVerlet;
    } else if (token == "gravity") {
      conf >> gravity;
    } else if (token == "density") {
      conf >> density;
    } else if (token == "kn") {
      conf >> kn;
    } else if (token == "kt") {
      conf >> kt;
    } else if (token == "dampRate") {
      conf >> dampRate;
    } else if (token == "mu") {
      conf >> mu;
    } else if (token == "nstor") {
      conf >> nstor;
    } else if (token == "iconf") {
      conf >> iconf;
    } else if (token == "Xperiod") {
      conf >> xmin >> xmax;
    } else if (token == "capture") {
      if (Particles.empty()) {
        std::cout << SPICE_WARN << "no far-field particles has been captured" << std::endl;
        std::cout << SPICE_INFO << "maybe 'capture' is placed before te definition of particles" << std::endl;
      }
      std::string which;
      conf >> which;
      if (which == "bottom") {
        double thickness;
        conf >> thickness >> bottom.K;
        updateYrange();
        capture(bottom, ymin, ymin + thickness);
      } else if (which == "top") {
        double thickness;
        conf >> thickness >> top.K;
        updateYrange();
        capture(top, ymax - thickness, ymax);
      }
    } else if (token == "bottom") {
      size_t nb;
      conf >> nb >> bottom.K;
      bottom.Idx.clear();
      bottom.pos.clear();
      size_t idx;
      vec2r pos;
      for (size_t i = 0; i < nb; i++) {
        conf >> idx >> pos;
        bottom.Idx.push_back(idx);
        bottom.pos.push_back(pos);
      }
    } else if (token == "top") {
      size_t nb;
      conf >> nb >> top.K;
      top.Idx.clear();
      top.pos.clear();
      size_t idx;
      vec2r pos;
      for (size_t i = 0; i < nb; i++) {
        conf >> idx >> pos;
        top.Idx.push_back(idx);
        top.pos.push_back(pos);
      }
    } else if (token == "Loading") {
      std::string loadingName;
      conf >> loadingName;
      if (Load != nullptr) {  // Load already exist
        getline(conf, token); // ignore the rest of the current line
        conf >> token;        // next token
        continue;
      }
      Load = Loading::create(loadingName.c_str());
      if (Load == nullptr) {
        std::cout << "Could not create the Loading" << std::endl;
      } else {
        Load->box = this;
        Load->read(conf);
      }

    } else if (token == "Particles") {
      size_t nb;
      conf >> nb;
      Particles.clear();
      Particle P;
      for (size_t i = 0; i < nb; i++) {
        conf >> P.pos >> P.vel >> P.acc >> P.rot >> P.vrot >> P.arot >> P.radius >> P.inertia >> P.mass;
        Particles.push_back(P);
      }
    } else if (token == "Interactions") {
      size_t nb;
      conf >> nb;
      Interactions.clear();
      Interaction I;
      for (size_t i = 0; i < nb; i++) {
        conf >> I.i >> I.j >> I.fn >> I.ft >> I.damp;
        Interactions.push_back(I);
      }
    } else {
      std::cout << SPICE_WARN << "Unknown token: " << token << std::endl;
    }

    conf >> token;
  }

  // some checks
  if (Particles.empty()) { std::cout << SPICE_WARN << "No Particles" << std::endl; }

  if (Load == nullptr) { std::cout << SPICE_WARN << "No Loading defined!" << std::endl; }
}

// ---------------------------------------------------------
// ---------------------------------------------------------
void SPICE::updateYrange() {
  ymin = Particles[0].pos.y - Particles[0].radius;
  ymax = Particles[0].pos.y + Particles[0].radius;
  for (size_t i = 1; i < Particles.size(); ++i) {
    double y = Particles[i].pos.y + Particles[i].radius;
    if (y > ymax) { ymax = y; }
    y = Particles[i].pos.y - Particles[i].radius;
    if (y < ymin) { ymin = y; }
  }
}

// ---------------------------------------------------------
// ---------------------------------------------------------
void SPICE::capture(FarField &field, double hmin, double hmax) {
  field.Idx.clear();
  field.pos.clear();
  for (size_t i = 0; i < Particles.size(); i++) {
    if (Particles[i].pos.y >= hmin && Particles[i].pos.y <= hmax) {
      field.Idx.push_back(i);
      field.pos.push_back(Particles[i].pos);
    }
  }
}

// ---------------------------------------------------------
// ---------------------------------------------------------
void SPICE::buildSampleInteractively() {
  Particle P;
  P.rot = 0.0;

  int ngw = 15;
  std::cout << std::endl;
  std::cout << "Number of spheres along width? ";
  std::cin >> ngw;

  double radius = 1e-3;
  std::cout << "Maximum radius? ";
  std::cin >> radius;
  double deltaR = 0.2e-3;
  std::cout << "Radius variation? ";
  std::cin >> deltaR;
  vec2r from(0.0, 0.0);
  vec2r to(2.0 * (ngw + 1) * radius, 2.0 * (ngw + 1) * radius);
  dVerlet = 0.95 * (radius - deltaR); // 95% of smallest radius

  int i          = 0;
  double massTot = 0.0;
  double step    = to.x / (2.0 * ngw);
  while (P.pos.y < to.x) {
    P.radius = radius - deltaR * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    P.mass   = M_PI * P.radius * P.radius * density;
    massTot += P.mass;
    P.inertia  = 0.5 * P.mass * P.radius * P.radius;
    int column = i % ngw;
    int row    = i / ngw;
    if (row % 2 == 0) { // even row
      P.pos.x = step + 2 * column * step;
    } else { // odd row
      P.pos.x = 2 * step + 2 * column * step;
    }
    P.pos.y = step + 2 * row * step;
    if (P.pos.y <= to.y - step) { Particles.push_back(P); }
    i++;
  }

  // recompute the bounding box
  from.set(1e20, 1e20);
  to.set(-1e20, -1e20);
  vec2r diag(1.0, 1.0);
  for (size_t i = 0; i < Particles.size(); i++) {
    vec2r pmin = Particles[i].pos - Particles[i].radius * diag;
    vec2r pmax = Particles[i].pos + Particles[i].radius * diag;
    if (from.x > pmin.x) { from.x = pmin.x; }
    if (from.y > pmin.y) { from.y = pmin.y; }
    if (to.x < pmax.x) { to.x = pmax.x; }
    if (to.y < pmax.y) { to.y = pmax.y; }
  }
  // period in x direction
  xmin = from.x;
  xmax = to.x;

  // Far field attached particles
  bottom.Idx.clear();
  bottom.pos.clear();
  bottom.K = 1000;
  top.Idx.clear();
  top.pos.clear();
  top.K = 1000;
  for (size_t i = 0; i < Particles.size(); i++) {
    if (Particles[i].pos.y < from.y + 2.0 * radius) {
      bottom.Idx.push_back(i);
      bottom.pos.push_back(Particles[i].pos);
    } else if (Particles[i].pos.y > to.y - 2.0 * radius) {
      top.Idx.push_back(i);
      top.pos.push_back(Particles[i].pos);
    }
  }

  // Set ramdom velocities to particles
  double vmax = 0.1;
  std::cout << "Norm of ramdom velocities? ";
  std::cin >> vmax;
  for (size_t i = 0; i < Particles.size(); i++) {
    Particles[i].vel.x = vmax * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    Particles[i].vel.y = vmax * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
  }

  double press = 1000.0;
  std::cout << "Pressure? ";
  std::cin >> press;
  Load = Loading::create("ShearVV"); // bon il n'y a pas de pression pour le moment

  // Parametres de simu

  density = 2700.0;
  std::cout << "Particle densities? ";
  std::cin >> density;

  kn = 1.e4;
  while (true) {
    char rep = 'y';
    std::cout << "kn? ";
    std::cin >> kn;
    std::cout << "kn/p = " << kn / press << std::endl;
    std::cout << "       Is it ok? (y/n) ";
    std::cin >> rep;
    if (rep == 'y' || rep == 'Y' || rep == 'o' || rep == 'O') { break; }
  }

  double ktkn = 1.0;
  std::cout << "kt/kn? ";
  std::cin >> ktkn;
  kt = ktkn * kn;

  dampRate = 0.95;
  std::cout << "Viscuous damping rate? ";
  std::cin >> dampRate;

  mu = 0.8;
  std::cout << "Friction coefficient? ";
  std::cin >> mu;

  dt = 1e-6;
  while (true) {
    char rep = 'y';
    std::cout << "dt? ";
    std::cin >> dt;
    double m = M_PI * radius * radius * density;
    std::cout << "dt/dt_crit = " << dt / (sqrt(m / kn)) << std::endl;
    std::cout << "             Is it ok? (y/n) ";
    std::cin >> rep;
    if (rep == 'y' || rep == 'Y' || rep == 'o' || rep == 'O') { break; }
  }

  t           = 0.0;
  nstor       = 0;
  iconf       = 0;
  interLookC  = 0.0;
  interCloseC = 0.0;
  interOutC   = 0.0;
  interHistC  = 0.0;
  tmax        = 5.0;
  std::cout << "tmax? ";
  std::cin >> tmax;

  // TODO
  interLook  = 0.25;
  interClose = 0.01;
  interOut   = 0.1;
  interHist  = 0.25;

  return;
}

// ---------------------------------------------------------
// ---------------------------------------------------------
void SPICE::screenLog() {
  std::cout << std::endl;
  std::cout << "-----------------------------------" << std::endl;
  std::cout << " iconf = " << iconf << ", time = " << t << std::endl;
  std::cout << "-----------------------------------" << std::endl;
}

// ---------------------------------------------------------
// The integration loop (Velocity Verlet)
// ---------------------------------------------------------
void SPICE::integrate() {
  double dt_2  = 0.5 * dt;
  double dt2_2 = 0.5 * dt * dt;

  std::ofstream stressOut("stress.out.txt");
  double Lperiod = xmax - xmin;
  while (t < tmax) {

    Load->servo();

    for (size_t i = 0; i < Particles.size(); i++) {
      Particles[i].pos += dt * Particles[i].vel + dt2_2 * Particles[i].acc;
      if (Particles[i].pos.x > xmax) {
        Particles[i].pos.x -= Lperiod;
      } else if (Particles[i].pos.x < xmin) {
        Particles[i].pos.x += Lperiod;
      }

      Particles[i].vel += dt_2 * Particles[i].acc;

      Particles[i].rot += dt * Particles[i].vrot + dt2_2 * Particles[i].arot;
      Particles[i].vrot += dt_2 * Particles[i].arot;
    }

    Load->velocityVerlet_halfStep1(dt);

    accelerations();

    for (size_t i = 0; i < Particles.size(); i++) {
      Particles[i].vel += dt_2 * Particles[i].acc;
      Particles[i].vrot += dt_2 * Particles[i].arot;
    }

    Load->velocityVerlet_halfStep2(dt);

    if (interCloseC >= interClose) {
      resetCloseList(dVerlet);
      interCloseC = 0.0;
    }

    /*
    if (interOutC >= interOut) {
      stressOut << t << " " << Sigxx << " " << Sigxy << " " << Sigyx << " " << Sigyy << std::endl;
      interOutC = 0.0;
    }
    */

    if (interHistC >= interHist) {
      saveConf(iconf++);
      screenLog();
      interHistC = 0.0;
    }

    interHistC += dt;
    interOutC += dt;
    interLookC += dt;
    interCloseC += dt;
    t += dt;
  }

  return;
}

// ---------------------------------------------------------
// ---------------------------------------------------------
double SPICE::getBranchShift(double xbranch, double Lperiod) {
  if (xbranch > 0.5 * Lperiod) {
    return -Lperiod;
  } else if (xbranch < -0.5 * Lperiod) {
    return Lperiod;
  }
  return 0.0;
}

// ---------------------------------------------------------
// O(N^2) algorithm
// ---------------------------------------------------------
void SPICE::resetCloseList(double dmax) {
  // store ft because the list will be erased before being rebuilt
  struct ftbak_t {
    size_t i, j;
    double ft;
  };

  std::vector<ftbak_t> ftbak;
  ftbak_t I;
  for (size_t k = 0; k < Interactions.size(); ++k) {
    I.i  = Interactions[k].i;
    I.j  = Interactions[k].j;
    I.ft = Interactions[k].ft;
    ftbak.push_back(I);
  }

  // now rebuild the list
  Interactions.clear();
  double Lperiod = xmax - xmin;
  for (size_t i = 0; i < Particles.size(); ++i) {
    for (size_t j = i + 1; j < Particles.size(); ++j) {

      vec2r branch = Particles[j].pos - Particles[i].pos;
      branch.x += getBranchShift(branch.x, Lperiod);

      double sum = dmax + Particles[i].radius + Particles[j].radius;
      if (norm2(branch) <= sum * sum) {
        double m    = (Particles[i].mass * Particles[j].mass) / (Particles[i].mass + Particles[j].mass);
        double Damp = 2.0 * dampRate * sqrt(kn * m);
        Interactions.push_back(Interaction(i, j, Damp));
      }
    }
  }

  // retrieve ft values
  size_t k, kold = 0;
  for (k = 0; k < Interactions.size(); ++k) {
    while (kold < ftbak.size() && ftbak[kold].i < Interactions[k].i) { ++kold; }
    if (kold == ftbak.size()) { break; }

    while (kold < ftbak.size() && ftbak[kold].i == Interactions[k].i && ftbak[kold].j < Interactions[k].j) { ++kold; }
    if (kold == ftbak.size()) { break; }

    if (ftbak[kold].i == Interactions[k].i && ftbak[kold].j == Interactions[k].j) {
      Interactions[k].ft = ftbak[kold].ft;
      ++kold;
    }
  }
}

// ---------------------------------------------------------
// ---------------------------------------------------------
void SPICE::accelerations() {
  // Set forces and moments to zero
  for (size_t i = 0; i < Particles.size(); ++i) {
    Particles[i].force.reset();
    Particles[i].moment = 0.0;
  }
  Sig.reset();

  double Lperiod = xmax - xmin;
  for (size_t k = 0; k < Interactions.size(); ++k) {
    size_t i = Interactions[k].i;
    size_t j = Interactions[k].j;

    vec2r branch = Particles[j].pos - Particles[i].pos;
    branch.x += getBranchShift(branch.x, Lperiod);

    double sum = Particles[i].radius + Particles[j].radius;
    if (norm2(branch) <= sum * sum) { // it means that i and j are in contact

      // real relative velocities
      vec2r realVel = Particles[j].vel - Particles[i].vel; // Does not account for rotation yet

      // Normal force (elastic + viscuous)
      vec2r n            = branch;
      double len         = n.normalize();
      double dn          = len - Particles[i].radius - Particles[j].radius;
      double vn          = realVel * n;
      double fne         = -kn * dn;
      double fnv         = -Interactions[k].damp * vn;
      Interactions[k].fn = fne + fnv;
      if (Interactions[k].fn < 0.0) { Interactions[k].fn = 0.0; }

      // Tangential force (friction)
      vec2r t(-n.y, n.x);
      double Ri    = Particles[i].radius + 0.5 * dn;
      double Rj    = Particles[j].radius + 0.5 * dn;
      double vijt  = realVel * t - Particles[i].vrot * Ri - Particles[j].vrot * Rj;
      double ft    = Interactions[k].ft - kt * dt * vijt;
      double ftest = mu * Interactions[k].fn;
      if (fabs(ft) > ftest) { ft = (ft > 0.0) ? ftest : -ftest; }
      Interactions[k].ft = ft;

      // Resultant force and moment
      vec2r f = Interactions[k].fn * n + Interactions[k].ft * t;
      Particles[i].force -= f;
      Particles[j].force += f;
      Particles[i].moment -= ft * Ri;
      Particles[j].moment -= ft * Rj;

      // Internal stress
      Sig.xx += f.x * branch.x;
      Sig.xy += f.x * branch.y;
      Sig.yx += f.y * branch.x;
      Sig.yy += f.y * branch.y;
    } // if
  } // Loop over interactions

  // bottom far-stiffness
  for (size_t m = 0; m < bottom.Idx.size(); ++m) {
    size_t idx = bottom.Idx[m];
    Particles[idx].force -= bottom.K * (Particles[idx].pos - bottom.pos[m]);
  }

  // top far-stiffness
  for (size_t m = 0; m < top.Idx.size(); ++m) {
    size_t idx = top.Idx[m];
    Particles[idx].force -= top.K * (Particles[idx].pos - top.pos[m]);
  }

  updateYrange();
  double invV = 1.0 / ((xmax - xmin) * (ymax - ymin));
  Sig *= invV;

  // Finally compute the accelerations (translation and rotation)
  for (size_t i = 0; i < Particles.size(); i++) {
    Particles[i].acc  = Particles[i].force / Particles[i].mass + gravity;
    Particles[i].arot = Particles[i].moment / Particles[i].inertia;
  }

  Load->forceDrivenAcceleration();
}
