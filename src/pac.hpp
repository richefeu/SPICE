// Packing And Configuration
#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include "SPICE.hpp"

#include "farConnectionManager.hpp"
#include "pacOptionsManager.hpp"
#include "packingManager.hpp"
#include "propertiesManager.hpp"
#include "propertyProfile.hpp"

// toofus headers
#include "toofus/fileTool.hpp"

void readJsonFile(const char *filename, pacOptionsManager &, packingManager &, farConnectionManager &,
                  propertiesManager &);
