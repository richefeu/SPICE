// ....

#include "SPICE.hpp"

SPICE::SPICE() {
  // registration of parameters embedded in particles that can be defined with a profile
  MemberAccessor<Particle>::registerMember("normalStiffness", &Particle::normalStiffness);
  MemberAccessor<Particle>::registerMember("tangentialStiffness", &Particle::tangentialStiffness);
  MemberAccessor<Particle>::registerMember("normalViscDampingRate", &Particle::normalViscDampingRate);
  MemberAccessor<Particle>::registerMember("friction", &Particle::friction);
  MemberAccessor<Particle>::registerMember("rollingFriction", &Particle::rollingFriction);
  MemberAccessor<Particle>::registerMember("adhesion", &Particle::adhesion);
  MemberAccessor<Particle>::registerMember("GcGlue", &Particle::GcGlue);
}

// ---------------------------------------------------------
// ---------------------------------------------------------
void SPICE::head() {
  std::cout << '\n';
  std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⡾⠃ " << '\n';
  std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⣾⠋⠀⠀ " << '\n';
  std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠛⠻⢿⣷⣄⠀⠀ " << '\n';
  std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡾⢛⣿⣿⣶⣄⠙⠿⠀⠀      SPICE - " << SPICE_VERSION << '\n';
  std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡟⢀⣾⣿⣿⣿⣿⣷⡀⠀⠀      Laboratoire 3SR" << '\n';
  std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡿⢀⣾⣿⣿⣿⣿⣿⣿⠀⠀⠀      Université Grenoble Alpes, France" << '\n';
  std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⡇⣼⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀          vincent.richefeu@univ-grenoble-alpes.fr" << '\n';
  std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣷⣿⣿⣿⣿⣿⡿⠁⠀⠀⠀⠀          cyrille.couture@univ-grenoble-alpes.fr" << '\n';
  std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⠏⠀⠀⠀⠀⠀⠀ " << '\n';
  std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣾⣿⣿⣿⣿⡿⠟⠁⠀⠀⠀⠀⠀⠀⠀ " << '\n';
  std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣶⣿⣿⣿⣿⠿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ " << '\n';
  std::cout << "⠀⠀⠀⠀⠀⠀⢀⣤⣾⣿⣿⣿⠿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ " << '\n';
  std::cout << "⠀⠀⠀⠀⣠⣶⡿⠿⠛⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ " << '\n';  
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

  conf << "SPICE " << SPICE_VERSION << std::endl; // format: progName version-date

  conf << std::setprecision(6);
  conf << "t " << t << std::endl;
  conf << "tmax " << tmax << std::endl;
  conf << "dt " << dt << std::endl;
  conf << "interClose " << interClose << std::endl;
  conf << "interOut " << interOut << std::endl;
  conf << "interHist " << interHist << std::endl;
  conf << "dVerlet " << dVerlet << std::endl;
  conf << "gravity " << gravity << std::endl;
  conf << "iconf " << iconf << std::endl;
  conf << "Xperiod " << xmin << ' ' << xmax << std::endl;

  conf << "bottom " << bottom.Idx.size() << ' ' << bottom.K << std::endl;
  conf << std::setprecision(15);
  for (size_t i = 0; i < bottom.Idx.size(); i++) { conf << bottom.Idx[i] << ' ' << bottom.pos[i] << std::endl; }

  conf << std::setprecision(6);
  conf << "top " << top.Idx.size() << ' ' << top.K << std::endl;
  conf << std::setprecision(15);
  for (size_t i = 0; i < top.Idx.size(); i++) { conf << top.Idx[i] << ' ' << top.pos[i] << std::endl; }

  conf << "Loading ";
  if (Load != nullptr) {
    Load->write(conf);
  } else {
    std::cout << SPICE_WARN << "Loading is not defined" << std::endl;
    conf << "!! TO BE ADDED !!" << std::endl;
  }

  conf << "Particles " << Particles.size() << std::endl;
  conf << std::setprecision(15);
  for (size_t i = 0; i < Particles.size(); i++) {
    conf << Particles[i].pos << ' ' << Particles[i].vel << ' ' << Particles[i].acc << ' ' << Particles[i].rot << ' '
         << Particles[i].vrot << ' ' << Particles[i].arot << ' ' << Particles[i].radius << ' ' << Particles[i].inertia
         << ' ' << Particles[i].mass << ' ';
    conf << Particles[i].normalStiffness << ' ' << Particles[i].tangentialStiffness << ' '
         << Particles[i].normalViscDampingRate << ' ' << Particles[i].friction << ' ' << Particles[i].rollingFriction
         << ' ' << Particles[i].adhesion << ' ' << Particles[i].GcGlue << std::endl;
  }

  size_t nbInteractions = 0;
  for (size_t i = 0; i < Interactions.size(); i++) {
    if (fabs(Interactions[i].fn) < 1e-20 && Interactions[i].isBonded == false) { continue; }
    ++nbInteractions;
  }
  conf << "Interactions " << nbInteractions << std::endl;
  conf << std::setprecision(15);
  for (size_t i = 0; i < Interactions.size(); i++) {
    if (fabs(Interactions[i].fn) < 1e-20 && Interactions[i].isBonded == false) { continue; }
    conf << Interactions[i].i << ' ' << Interactions[i].j << ' ' << Interactions[i].isBonded << ' '
         << Interactions[i].isSameMaterialBond << ' ' << Interactions[i].fn << ' ' << Interactions[i].fnb << ' '
         << Interactions[i].ft << ' ' << Interactions[i].ftb << ' ' << Interactions[i].damp << ' ' << Interactions[i].Gs
         << ' ' << Interactions[i].dn0 << std::endl;
  }
}

// ---------------------------------------------------------
// A fast way to load 'conf<i>' files (just by specifying i)
// ---------------------------------------------------------
void SPICE::loadConf(int i) {
  char fname[256];
  sprintf(fname, "conf%d", i);
  loadConf(fname);
}

// ---------------------------------------------------------
// Loading a conf-file by giving its name path
// ---------------------------------------------------------
void SPICE::loadConf(const char *name) {
  std::ifstream conf(name);
  if (!conf.is_open()) { std::cout << SPICE_WARN << "Cannot read " << name << std::endl; }

  // A warning for the placement of procesing commands
  auto warn_if_wrong_place = [&](const std::string &token) {
    if (Particles.empty()) {
      std::cout << SPICE_WARN << "the command " << token << " should be placed after the definition of particles"
                << std::endl;
    }
  };

  // Check header
  std::string prog;
  conf >> prog;
  if (prog != "SPICE") { std::cout << SPICE_WARN << "This is not file for SPICE executable!" << std::endl; }
  std::string date;
  conf >> date;
  if (date != SPICE_VERSION) { std::cout << SPICE_WARN << "The version-date should be " << SPICE_VERSION << std::endl; }

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
    } else if (token == "iconf") {
      conf >> iconf;
    } else if (token == "Xperiod") {
      conf >> xmin >> xmax;
    } else if (token == "capture") {
      if (Particles.empty()) {
        std::cout << SPICE_WARN << "no far-connection particles can be captured" << std::endl;
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
      if (loadingName[0] == '!') {
        std::cout << SPICE_WARN << "Remember to define the Loading" << std::endl;
        getline(conf, token); // ignore the rest of the current line
        conf >> token;        // next token
        continue;
      }
      Load = Loading::create(loadingName.c_str());
      if (Load == nullptr) {
        std::cout << SPICE_WARN << "Could not create the Loading" << std::endl;
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
        conf >> P.normalStiffness >> P.tangentialStiffness >> P.normalViscDampingRate >> P.friction >>
            P.rollingFriction >> P.adhesion >> P.GcGlue;

        Particles.push_back(P);
      }
    } else if (token == "Interactions") {
      size_t nb;
      conf >> nb;
      Interactions.clear();
      Interaction I;
      for (size_t k = 0; k < nb; k++) {
        conf >> I.i >> I.j >> I.isBonded >> I.isSameMaterialBond >> I.fn >> I.fnb >> I.ft >> I.ftb >> I.damp >> I.Gs >>
            I.dn0;
        Interactions.push_back(I);
      }
    }

    // Processing commands that should be placed after
    // the definition of particles (at the very end of the conf-file preferably).
    // This commands generally added in input-files but not saved in the conf-files
    else if (token == "activateBonds") {
      warn_if_wrong_place(token);
      bool isSameMaterial;
      double distanceMaxForGluing;
      conf >> isSameMaterial >> distanceMaxForGluing;
      activateBonds(isSameMaterial, distanceMaxForGluing);
    }

    // EmbeddedDataProfile kn 1  0 10000
    // EmbeddedDataProfile mu 2  0.0 0.0   1.0 0.8

    // std::map<std::string, propertyProfile<double>> profile;
    // e.g. profile["kn"] ...

    else if (token == "EmbeddedDataProfile") {
      std::string param;
      conf >> param;

      if (!MemberAccessor<Particle>::hasRegistered(param)) {
        std::cout << SPICE_WARN << "parameters '" << param << "' cannot define a profile" << std::endl;
        getline(conf, token); // ignore the rest of the current line
        conf >> token;        // next token
        continue;
      }

      auto it = EmbeddedDataProfileMap.find(param);
      if (it != EmbeddedDataProfileMap.end()) {
        EmbeddedDataProfileMap.erase(it);
        std::cout << SPICE_WARN << "profile " << param << " was already defined. It will be redefined" << std::endl;
      }

      propertyProfile<double> P;
      P.readStream(conf);
      EmbeddedDataProfileMap[param] = P;
      std::cout << SPICE_INFO << "profile for '" << param << "' -> " << P << std::endl;
    }

    // Unknown token
    else {
      std::cout << SPICE_WARN << "Unknown token: " << token << std::endl;
    }

    conf >> token;
  }

  // precompute things ========================================
  updateYrange();
  applyEmbeddedDataProfiles(); // set the property profiles that have been eventually (re-)set
  combineParameters();

  // some checks
  if (Particles.empty()) { std::cout << SPICE_WARN << "No Particles" << std::endl; }

  if (Load == nullptr) { std::cout << SPICE_WARN << "No Loading defined!" << std::endl; }
}

// ---------------------------------------------------------
//
// ---------------------------------------------------------
void SPICE::applyEmbeddedDataProfiles() {
  for (const auto &entry : EmbeddedDataProfileMap) {
    std::string param               = entry.first;
    propertyProfile<double> profile = entry.second;

    size_t offset = MemberAccessor<Particle>::getOffset(param);
    double invH = 1.0 / (ymax - ymin);
    for (size_t i = 0; i < Particles.size(); ++i) {
      double normalizedHeight = invH * (Particles[i].pos.y - ymin);
      double value            = profile.getValueAt(normalizedHeight);
      MemberAccessor<Particle>::setAtOffset<double>(offset, Particles[i], value);
    }
  }
}

// ---------------------------------------------------------
//
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
//
// ---------------------------------------------------------
void SPICE::activateBonds(bool sameMaterial, double dmax) {
  // In case the conf-file has no interactions, the neighbor list is updated
  resetCloseList(dmax);

  double Lperiod = xmax - xmin;

  std::cout << " routine to activate bonds" << std::endl;

  for (size_t k = 0; k < Interactions.size(); k++) {
    size_t i = Interactions[k].i;
    size_t j = Interactions[k].j;

    vec2r branch = Particles[j].pos - Particles[i].pos;
    branch.x += getBranchShift(branch.x, Lperiod);

    double branchLen2 = norm2(branch);
    double sum        = dmax + Particles[i].radius + Particles[j].radius;
    if (branchLen2 <= sum * sum) {

      // switch to a cemented/bonded link
      Interactions[k].isBonded           = true;
      Interactions[k].isSameMaterialBond = sameMaterial;
      // TODO Use Gc and min diameter to define a threshold Wmax

      std::cout << " activate bond = " << k << std::endl;

      // double dn = sqrt(branchLen2) - (Particles[i].radius + Particles[j].radius);
      // if (dn >= 0.0) Interactions[k].dn0 = dn;
      // else Interactions[k].dn0 = 0.0;
      Interactions[k].dn0 = sqrt(branchLen2) - (Particles[i].radius + Particles[j].radius);

    } // endif
  } // end loop over interactions
}

// ---------------------------------------------------------
//
// ---------------------------------------------------------
void SPICE::updateTotalMass() {}

// ---------------------------------------------------------
//
// ---------------------------------------------------------
void SPICE::updateSizeRange() {}

// ---------------------------------------------------------
//
// ---------------------------------------------------------
void SPICE::capture(FarConnection &field, double hmin, double hmax) {
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
// Log periodically useful informations
// on how the computation goes
// ---------------------------------------------------------
void SPICE::screenLog() {
  std::cout << std::endl;
  std::cout << "————————————————————————————————————————————————————————————————" << std::endl;
  std::cout << " iconf = " << iconf << "/" << iconfMaxEstimated << ", time = " << t << std::endl;
  std::cout << " Stress-particles:  " << std::endl;
  Sig.fancyPrint(mat4r::ColoredBrackets | mat4r::Scientific, 12);
  std::cout << " Stress-connection: " << std::endl;
  SigConnect.fancyPrint(mat4r::ColoredBrackets | mat4r::Scientific, 12);
  // std::cout << " Stress-total:      " << Sig + SigConnect << std::endl;

  // ...
  std::cout << "————————————————————————————————————————————————————————————————" << std::endl;
}

// ---------------------------------------------------------
// Get the x-shift to manage periodic copies
// ---------------------------------------------------------
double SPICE::getBranchShift(double xbranch, double Lperiod) {
  if (xbranch > 0.5 * Lperiod) {
    return -Lperiod;
  } else if (xbranch < -0.5 * Lperiod) {
    return Lperiod;
  }
  return 0.0;
}

double SPICE::harmonicMean(double x, double y) {
  return x * y / (x + y);
}

// ---------------------------------------------------------
// Combines the parameters that are stored within particles
// of the interaction k
// ---------------------------------------------------------
void SPICE::combineParameters(size_t k) {

  size_t i = Interactions[k].i;
  size_t j = Interactions[k].j;

  // TODO with functionDispatcher in toofus
  // Interactions[k].kn = combineDispatcher.call(kn_dispatcher, Particles[i].kn, Particles[j].kn);

  Interactions[k].meff = harmonicMean(Particles[i].mass, Particles[j].mass);
  Interactions[k].kn   = harmonicMean(Particles[i].normalStiffness, Particles[j].normalStiffness);
  Interactions[k].kt   = harmonicMean(Particles[i].tangentialStiffness, Particles[j].tangentialStiffness);
  double dampingRate   = harmonicMean(Particles[i].normalViscDampingRate, Particles[j].normalViscDampingRate);
  Interactions[k].damp = 2.0 * dampingRate * sqrt(Interactions[k].kn * Interactions[k].meff);
  Interactions[k].mu   = std::min(Particles[i].friction, Particles[j].friction);
  Interactions[k].muR  = harmonicMean(Particles[i].rollingFriction, Particles[j].rollingFriction);
  Interactions[k].fadh = std::min(Particles[i].adhesion, Particles[j].adhesion);
  // ...
}

// ---------------------------------------------------------
// Combines the parameters that are stored within particles
// for all Interactions
// ---------------------------------------------------------
void SPICE::combineParameters() {
  for (size_t k = 0; k < Interactions.size(); ++k) { combineParameters(k); }
}

// ---------------------------------------------------------
// O(N^2) algorithm for updating the list of neighbors
// of each particle
// ---------------------------------------------------------
void SPICE::resetCloseList(double dmax) { // TODO: rename updateNeighborList()

  // store ft because the list will be erased before being rebuilt
  std::vector<Interaction> storedInteractions(Interactions.size());
  std::copy(Interactions.begin(), Interactions.end(), storedInteractions.begin());

  // now clear the list and rebuild it
  Interactions.clear();
  double Lperiod = xmax - xmin;
  for (size_t i = 0; i < Particles.size(); ++i) {
    for (size_t j = i + 1; j < Particles.size(); ++j) {

      vec2r branch = Particles[j].pos - Particles[i].pos;
      branch.x += getBranchShift(branch.x, Lperiod);

      double sum = dmax + Particles[i].radius + Particles[j].radius;
      if (norm2(branch) <= sum * sum) { Interactions.push_back(Interaction(i, j)); }
    }
  }

  // retrieve the embbeded values from Interactions that were present
  size_t k{0}, kold{0};
  for (; k < Interactions.size(); ++k) {
    while (kold < storedInteractions.size() && storedInteractions[kold].i < Interactions[k].i) { ++kold; }
    if (kold == storedInteractions.size()) { break; }

    while (kold < storedInteractions.size() && storedInteractions[kold].i == Interactions[k].i &&
           storedInteractions[kold].j < Interactions[k].j) {
      ++kold;
    }
    if (kold == storedInteractions.size()) { break; }

    if (storedInteractions[kold].i == Interactions[k].i && storedInteractions[kold].j == Interactions[k].j) {
      Interactions[k].copy(storedInteractions[kold]);
      ++kold;
    } else {
      combineParameters(k);
    }
  }

  // finish parameter-combinations in case of loop-break
  if (k < Interactions.size()) {
    if (k > 0) { --k; }
    for (; k < Interactions.size(); ++k) { combineParameters(k); }
  }
}

// ---------------------------------------------------------
// The integration loop (Velocity Verlet)
// ---------------------------------------------------------
void SPICE::integrate() {
  double dt_2  = 0.5 * dt;
  double dt2_2 = 0.5 * dt * dt;

  iconfMaxEstimated = iconf + floor((tmax - t) / interHist);

  std::ofstream stressOut("stress.out.txt");
  // std::ofstream debug("debug.txt");

  double Lperiod = xmax - xmin;

  Load->init();

  saveConf(iconf); // save before any computation
  screenLog();
  ++iconf;
  interHistC = 0.0;

  std::cout << SPICE_INFO << "Beginning iterations." << std::endl;
  while (t < tmax) {

    Load->servo();

    Load->velocityVerlet_halfStep1();
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

    accelerations();

    Load->velocityVerlet_halfStep2();
    for (size_t i = 0; i < Particles.size(); i++) {
      Particles[i].vel += dt_2 * Particles[i].acc;
      Particles[i].vrot += dt_2 * Particles[i].arot;
    }

    t += dt;

    interCloseC += dt;
    if (interCloseC > interClose - dt_2) {
      resetCloseList(dVerlet);
      interCloseC = 0.0;
    }

    interOutC += dt;
    if (interOutC > interOut - dt_2) {
      stressOut << t << " " << Sig << " " << SigConnect << std::endl;
      interOutC = 0.0;
      /*
      if (!Interactions.empty())
        debug << t << ' ' <<  Interactions[0].i << ' ' << Interactions[0].j << ' ' << Interactions[0].ft << ' '
              << Interactions[0].fn << std::endl;
      */
    }

    interHistC += dt;
    if (interHistC > interHist - dt_2) {
      saveConf(iconf);
      screenLog();
      ++iconf;
      interHistC = 0.0;
    }
  }

  return;
}

// ---------------------------------------------------------
// Compute the accelerations
// ---------------------------------------------------------
void SPICE::accelerations() {
  // Set forces and moments to zero
  for (size_t i = 0; i < Particles.size(); ++i) {
    Particles[i].force.reset();
    Particles[i].moment = 0.0;
    Particles[i].acc    = gravity;
    Particles[i].arot   = 0.0;
  }
  Sig.reset();
  SigConnect.reset();

  computeForcesAndMoments();
  computeFarConnectionForces();

  updateYrange();
  double invV = 1.0 / ((xmax - xmin) * (ymax - ymin));
  Sig *= invV;
  SigConnect *= invV;

  // Finally compute the accelerations (translation and rotation)
  Load->forceDrivenAcceleration();
  for (size_t i = 0; i < Particles.size(); i++) {
    Particles[i].acc  = gravity + Particles[i].force / Particles[i].mass;
    Particles[i].arot = Particles[i].moment / Particles[i].inertia;
  }
}

// ---------------------------------------------------------
// FIXME: rename FAR -> SOFT (or nothing)
// ---------------------------------------------------------
void SPICE::computeFarConnectionForces() {
  double Lperiod = xmax - xmin;

  // bottom soft-connection-stiffness
  for (size_t m = 0; m < bottom.Idx.size(); ++m) {
    size_t idx   = bottom.Idx[m];
    vec2r branch = Particles[idx].pos - bottom.pos[m];
    branch.x += getBranchShift(branch.x, Lperiod);
    vec2r f = bottom.K * branch;
    Particles[idx].force -= f;
    // Particles[idx].moment -= bottom.Kr * Particles[idx].rot;
    SigConnect.xx += f.x * branch.x;
    SigConnect.xy += f.x * branch.y;
    SigConnect.yx += f.y * branch.x;
    SigConnect.yy += f.y * branch.y;
  }

  // top soft-connection-stiffness
  for (size_t m = 0; m < top.Idx.size(); ++m) {
    size_t idx   = top.Idx[m];
    vec2r branch = Particles[idx].pos - top.pos[m];
    branch.x += getBranchShift(branch.x, Lperiod);
    vec2r f = top.K * branch;
    Particles[idx].force -= f;
    SigConnect.xx += f.x * branch.x;
    SigConnect.xy += f.x * branch.y;
    SigConnect.yx += f.y * branch.x;
    SigConnect.yy += f.y * branch.y;
  }
}

// ---------------------------------------------------------
//
// ---------------------------------------------------------
void SPICE::computeForcesAndMoments() {
  double Lperiod = xmax - xmin;
  for (size_t k = 0; k < Interactions.size(); ++k) {

    size_t i = Interactions[k].i;
    size_t j = Interactions[k].j;

    vec2r branch = Particles[j].pos - Particles[i].pos;
    branch.x += getBranchShift(branch.x, Lperiod);

    vec2r unit_n = branch;
    double len   = unit_n.normalize();
    vec2r relVel = Particles[j].vel - Particles[i].vel; // Does not account for rotation yet

    vec2r unit_t(-unit_n.y, unit_n.x);
    double dn = len - Particles[i].radius - Particles[j].radius;
    double vn = relVel * unit_n;

    double Li   = Particles[i].radius + 0.5 * dn;
    double Lj   = Particles[j].radius + 0.5 * dn;
    double vijt = relVel * unit_t - Particles[i].vrot * Li - Particles[j].vrot * Lj;

    // ===================================
    // BOND
    // ===================================
    if (Interactions[k].isBonded == true) { // i and j are bonded

      // FIXME: il faut ajouter les variables dnb et dtb

      // calculate the bonded forces
      Interactions[k].fnb = -Interactions[k].kn * (dn - Interactions[k].dn0);
      // Interactions[k].ftb = Interactions[k].ft - Interactions[k].kt * dt * vijt;

    } else { // i and j not bonded

      Interactions[k].fnb = 0;
      Interactions[k].ftb = 0;
    }

    // ===================================
    // CONTACT
    // ===================================
    if (Interactions[k].isBonded == true && Interactions[k].isSameMaterialBond == true) {

      // no frictional contact when the bonded particles are linked with the same material
      Interactions[k].fn = 0.0;
      Interactions[k].ft = 0.0;

    } else { // frictional contact (possibly in addition to bond)

      if (dn < 0.0) { // it means that i and j are in contact

        double fne = -Interactions[k].kn * dn;   // elastic normal force
        double fnv = -Interactions[k].damp * vn; // viscous normal force

        Interactions[k].fn = fne + fnv;
        if (Interactions[k].fn < 0.0) { Interactions[k].fn = 0.0; }

        // Tangential force (friction)
        double ft    = Interactions[k].ft - Interactions[k].kt * (dt * vijt);
        double ftest = Interactions[k].mu * Interactions[k].fn; // remember that fn >= 0
        if (fabs(ft) > ftest) { ft = (ft > 0.0) ? ftest : -ftest; }
        Interactions[k].ft = ft;

        // Adhesion
        // TODO:
        // Interactions[k].fn -= fadh;
      }
    }

    // ===================================
    // BREAKAGE OF BONDS
    // ===================================
    // TODO if (isBonded) ...

    // Resultant force and moment
    vec2r f = (Interactions[k].fn + Interactions[k].fnb) * unit_n + (Interactions[k].ft + Interactions[k].ftb) * unit_t;
    Particles[i].force -= f;
    Particles[j].force += f;
    Particles[i].moment -= (Interactions[k].ft + Interactions[k].ftb) * Li;
    Particles[j].moment -= (Interactions[k].ft + Interactions[k].ftb) * Lj;

    // Internal stress
    Sig.xx += f.x * branch.x;
    Sig.xy += f.x * branch.y;
    Sig.yx += f.y * branch.x;
    Sig.yy += f.y * branch.y;

  } // Loop over interactions
}