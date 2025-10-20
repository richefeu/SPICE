#include "pac.hpp"

#include <fstream>
#include <iostream>

// toofus headers
#include "fileTool.hpp"
#include "propertyProfile.hpp"
//#include "toofus-gate/nlohmann/json.hpp"

int main(int argc, char const *argv[]) {

  if (argc != 2) {
    std::cout << "usage: set <data.json>" << std::endl;
    return 0;
  }

  if (!fileTool::fileExists(argv[1])) {
    std::cout << "File " << argv[1] << "does not exist" << std::endl;
    return 0;
  }

  // read dataset stored in json format
  pacOptionsManager pacOptionsM;
  packingManager packingM;
  farConnectionManager farConnectionM;
  propertiesManager propertiesM;
  readJsonFile(argv[1], pacOptionsM, packingM, farConnectionM, propertiesM);

  // do the job
  SPICE box;
  packingM.process(box);
  farConnectionM.process(box);
  propertiesM.process(box);
  pacOptionsM.process(box);

  // save
  box.saveConf(pacOptionsM.generatedFileName.c_str());

  return 0;
}

void readJsonFile(const char *filename, pacOptionsManager &pacOptionsM, packingManager &packingM,
                  farConnectionManager &farConnectionM, propertiesManager &propertiesM) {
  std::ifstream jsonfile(filename);
  if (!jsonfile.is_open()) { std::cout << "json file '" << filename << "' cannot be opened" << std::endl; }

  try {
    nlohmann::json j = nlohmann::json::parse(jsonfile);

    // --------------------------------------------
    if (j.contains("pac-options")) {
      pacOptionsM.generatedFileName = j["pac-options"].value("generated-file-name", pacOptionsM.generatedFileName);
      if (pacOptionsM.verbose) {
        std::cout << SPICE_INFO << "generated file name: " << pacOptionsM.generatedFileName << std::endl;
      }

      pacOptionsM.loading = j["pac-options"].value("loading", pacOptionsM.loading);
      pacOptionsM.verbose = j["pac-options"].value("verbose", pacOptionsM.verbose);

      std::vector<double> Gdefault{pacOptionsM.gravity.x, pacOptionsM.gravity.y};
      std::vector<double> G = j["pac-options"].value("gravity", Gdefault);
      pacOptionsM.gravity.x = G[0];
      pacOptionsM.gravity.y = G[1];

      pacOptionsM.t    = j["pac-options"].value("time", pacOptionsM.t);
      pacOptionsM.tmax = j["pac-options"].value("end-time", pacOptionsM.tmax);
      pacOptionsM.dt   = j["pac-options"].value("time-step", pacOptionsM.dt);

      pacOptionsM.interClose = j["pac-options"].value("time-between-update-neighbors", pacOptionsM.interClose);
      pacOptionsM.interOut   = j["pac-options"].value("time-between-outputs", pacOptionsM.interOut);
      pacOptionsM.interHist  = j["pac-options"].value("time-between-conf-files", pacOptionsM.interHist);
      pacOptionsM.dVerlet    = j["pac-options"].value("distance-alert", pacOptionsM.dVerlet);

      // "critical-time-step-divisor": 100.0
      if (j["pac-options"].contains("critical-time-step-divisor")) {
        // computeTimeStep = true;
        // criticalTimeStepDivisor = j["pac-options"]["critical-time-step-divisor"];
      }
    }

    // --------------------------------------------
    if (j.contains("packing-manager")) {
      packingM.option  = j["packing-manager"].value("option", packingM.option);
      packingM.nx      = j["packing-manager"].value("nx", packingM.nx);
      packingM.ny      = j["packing-manager"].value("ny", packingM.ny);
      packingM.density = j["packing-manager"].value("density", packingM.density);
      packingM.includeFarConnection =
          j["packing-manager"].value("include-far-connection", packingM.includeFarConnection);

      packingM.bottomNumber = j["packing-manager"].value("add-bottom-chain", 0);
      if (packingM.bottomNumber > 0) {
        packingM.hasBottomLine = true;
        // packingM.bottomThickness = 2.0 * bottomRadius;
      }

      packingM.topNumber = j["packing-manager"].value("add-top-chain", 0);
      if (packingM.topNumber > 0) {
        packingM.hasTopLine = true;
        // packingM.topThickness = 2.0 * topRadius;
      }

      if (j["packing-manager"].contains("radius")) {
        packingM.radius.readJson(j["packing-manager"]["radius"]);
      } else {
        packingM.radius.setConstant(1.0);
      }

      if (j["packing-manager"].contains("radius-variation")) {
        packingM.radiusVariation.readJson(j["packing-manager"]["radius-variation"]);
      } else {
        packingM.radiusVariation.setConstant(0.0);
      }
    }

    // --------------------------------------------
    if (j.contains("far-connection-manager")) {
      farConnectionM.option    = j["far-connection-manager"].value("option", farConnectionM.option);
      farConnectionM.location  = j["far-connection-manager"].value("location", farConnectionM.location);
      farConnectionM.thickness = j["far-connection-manager"].value("thickness", farConnectionM.thickness);
      farConnectionM.stiffness = j["far-connection-manager"].value("stiffness", farConnectionM.stiffness);
      farConnectionM.viscosity = j["far-connection-manager"].value("viscosity", farConnectionM.viscosity);
    }

    // --------------------------------------------
    if (j.contains("properties-manager")) {
      if (j["properties-manager"].contains("density")) {
        propertiesM.density.readJson(j["properties-manager"]["density"]);
        propertiesM.hasDensity = true;
      } else {
        propertiesM.density.setConstant(packingM.density);
        propertiesM.hasDensity = false;
      }

      if (j["properties-manager"].contains("friction")) {
        propertiesM.friction.readJson(j["properties-manager"]["friction"]);
        propertiesM.hasFriction = true;
      } else {
        propertiesM.friction.setConstant(0.5);
        propertiesM.hasFriction = false;
      }

      if (j["properties-manager"].contains("normalStiffness")) {
        propertiesM.normalStiffness.readJson(j["properties-manager"]["normalStiffness"]);
        propertiesM.hasNormalStiffness = true;
      } else {
        propertiesM.normalStiffness.setConstant(1e8);
        propertiesM.hasNormalStiffness = false;
      }

      if (j["properties-manager"].contains("tangentialStiffness")) {
        propertiesM.tangentialStiffness.readJson(j["properties-manager"]["tangentialStiffness"]);
        propertiesM.hasTangentialStiffness = true;
      } else {
        propertiesM.tangentialStiffness.setConstant(1e8);
        propertiesM.hasTangentialStiffness = false;
      }

      if (j["properties-manager"].contains("normalViscDampingRate")) {
        propertiesM.normalViscDampingRate.readJson(j["properties-manager"]["normalViscDampingRate"]);
        propertiesM.hasNormalViscDampingRate = true;
      } else {
        propertiesM.tangentialStiffness.setConstant(1e8);
        propertiesM.hasNormalViscDampingRate = false;
      }
    }

  } catch (const nlohmann::json::parse_error &e) {
    std::cerr << "JSON Parse Error:" << std::endl;
    std::cerr << "  Message: " << e.what() << std::endl;
    std::cerr << "  Byte position: " << e.byte << std::endl;
    std::cerr << "  Line: " << std::to_string(e.byte / 80 + 1) << std::endl; // Approximate line number
  }
}
