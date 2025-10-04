#include "pac.hpp"

#include <fstream>
#include <iostream>

// toofus things
#include "fileTool.hpp"
#include "propertyProfile.hpp"
#include "toofus-cpy/nlohmann/json.hpp"

void readJsonFile(const char *filename, pacOptionsManager &pacOptionsM, packingManager &packingM,
                  farConnectionManager &farConnectionM, propertiesManager &propertiesM) {
  std::ifstream jsonfile(filename);
  if (!jsonfile.is_open()) { std::cout << "json file '" << filename << "' cannot be opened" << std::endl; }

  try {
    nlohmann::json j = nlohmann::json::parse(jsonfile);

    // --------------------------------------------
    if (j.contains("pac-options")) {
      pacOptionsM.generatedFileName = j["pac-options"].value("generated-file-name", pacOptionsM.generatedFileName);
      pacOptionsM.loading           = j["pac-options"].value("loading", pacOptionsM.loading);
      pacOptionsM.verbose           = j["pac-options"].value("verbose", pacOptionsM.verbose);
      if (pacOptionsM.verbose) {
        std::cout << SPICE_INFO << "generated file name: " << pacOptionsM.generatedFileName << std::endl;
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
      farConnectionM.Young     = j["far-connection-manager"].value("Young", farConnectionM.Young);
      farConnectionM.viscosity = j["far-connection-manager"].value("viscosity", farConnectionM.viscosity);
    }

    // --------------------------------------------
    if (j.contains("properties-manager")) {
      if (j["properties-manager"].contains("density")) {
        propertiesM.density.readJson(j["properties-manager"]["density"]);
      } else {
        propertiesM.density.setConstant(packingM.density);
      }

      if (j["properties-manager"].contains("friction")) {
        propertiesM.friction.readJson(j["properties-manager"]["friction"]);
      } else {
        propertiesM.friction.setConstant(0.5);
      }

      if (j["properties-manager"].contains("stiffness")) {
        propertiesM.stiffness.readJson(j["properties-manager"]["stiffness"]);
      } else {
        propertiesM.stiffness.setConstant(1e8);
      }
    }

  } catch (const nlohmann::json::parse_error &e) {
    std::cerr << "JSON Parse Error:" << std::endl;
    std::cerr << "  Message: " << e.what() << std::endl;
    std::cerr << "  Byte position: " << e.byte << std::endl;
    std::cerr << "  Line: " << std::to_string(e.byte / 80 + 1) << std::endl; // Approximate line number
  }
}

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
  // farConnectionM.process(box);
  // propertiesM.process(box);
  pacOptionsM.process(box);
  
  // save
  box.saveConf(pacOptionsM.generatedFileName.c_str());

  return 0;
}