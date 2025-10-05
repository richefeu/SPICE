#include "propertiesManager.hpp"

void propertiesManager::process(SPICE &box) {

  if (box.Particles.empty()) {
    std::cout << SPICE_WARN << "You need to add particles to be able to set their properties" << std::endl;
  }

  box.updateYrange();

  if (hasDensity) {
    for (size_t i = 0; i < box.Particles.size(); i++) {
      double relativeHeight    = (box.Particles[i].pos.y - box.ymin) / (box.ymax - box.ymin);
      double particleDensity   = density.getValueAt(relativeHeight);
      double R                 = box.Particles[i].radius;
      box.Particles[i].mass    = M_PI * R * R * particleDensity;
      box.Particles[i].inertia = 0.5 * box.Particles[i].mass * R * R;
    }
  } else {
    if (box.verbose) {
      std::cout << SPICE_INFO
                << "Densities are not set in the 'properties-manager'.\n"
                   "You should set it in 'packing-manager' (same density for all particles)"
                << std::endl;
    }
  }

  if (hasNormalStiffness) {
    for (size_t i = 0; i < box.Particles.size(); i++) {
      double relativeHeight            = (box.Particles[i].pos.y - box.ymin) / (box.ymax - box.ymin);
      box.Particles[i].normalStiffness = normalStiffness.getValueAt(relativeHeight);
    }
  } else {
    if (box.verbose) { std::cout << SPICE_WARN << "No normal stiffness has been set" << std::endl; }
  }

  if (hasTangentialStiffness) {
    for (size_t i = 0; i < box.Particles.size(); i++) {
      double relativeHeight                = (box.Particles[i].pos.y - box.ymin) / (box.ymax - box.ymin);
      box.Particles[i].tangentialStiffness = tangentialStiffness.getValueAt(relativeHeight);
    }
  } else {
    if (box.verbose) { std::cout << SPICE_WARN << "No normal stiffness has been set" << std::endl; }
  }

  if (hasNormalViscDampingRate) {
    for (size_t i = 0; i < box.Particles.size(); i++) {
      double relativeHeight                  = (box.Particles[i].pos.y - box.ymin) / (box.ymax - box.ymin);
      box.Particles[i].normalViscDampingRate = normalViscDampingRate.getValueAt(relativeHeight);
    }
  } else {
    if (box.verbose) { std::cout << SPICE_WARN << "No Normal viscous damping rate has been set" << std::endl; }
  }

  if (hasFriction) {
    for (size_t i = 0; i < box.Particles.size(); i++) {
      double relativeHeight     = (box.Particles[i].pos.y - box.ymin) / (box.ymax - box.ymin);
      box.Particles[i].friction = friction.getValueAt(relativeHeight);
    }
  }
}