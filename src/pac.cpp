#include "pac.hpp"

#include <fstream>
#include <iostream>

// toofus things
#include "fileTool.hpp"
#include "propertyProfile.hpp"
#include "toofus-cpy/nlohmann/json.hpp"

void readJsonFile(const char *filename, packingModel &model) {
  std::ifstream jsonfile(filename);
  if (!jsonfile.is_open()) { std::cout << "json file '" << filename << "' cannot be opened" << std::endl; }

  try {
    nlohmann::json j = nlohmann::json::parse(jsonfile);

    model.name = j["packing-model"]["name"];
    model.nx   = j["packing-model"]["nx"];
    model.ny   = j["packing-model"]["ny"];
    model.radius.readJson(j["packing-model"]["radius"]);
    model.radiusVariation.readJson(j["packing-model"]["radius-variation"]);
    model.density              = j["packing-model"]["density"];
    model.includeFarConnection = j["packing-model"]["include-far-connection"];

    std::cout << "packing model: " << model.name << std::endl;
  } catch (const nlohmann::json::parse_error &e) {
    std::cerr << "Error parsing JSON: " << e.what() << std::endl;
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

  // read data
  packingModel model;
  readJsonFile(argv[1], model);

  // do the job
  SPICE box;
  model.pack(box);

  // save
  box.saveConf("input.txt");

  return 0;
}