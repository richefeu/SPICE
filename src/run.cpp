#include <cctype>
#include <iostream>

#include "SPICE.hpp"

// TODO move in somewhere into toofus 
bool containsOnlyDigits(const char *str) {
  while (*str != '\0') {       // Iterate through each character in the string
    if (!std::isdigit(*str)) { // Check if the current character is not a digit
      return false;            // If any non-digit character is found, return false
    }
    str++; // Move to the next character
  }
  return true; // Return true if all characters are digits
}

int main(int argc, char const *argv[]) {
  SPICE simu;
  simu.funnyHead();

  if (argc < 2) {
    simu.buildSampleInteractively();
    simu.saveConf("input.txt");
    return 0;
  } else {
    if (containsOnlyDigits(argv[1])) {
      int num = std::atoi(argv[1]);
      std::cout << "load conf" << num << std::endl;
			simu.loadConf(num);
    } else {
			std::cout << "load " << argv[1] << std::endl;
      simu.loadConf(argv[1]);
    }
  }

  simu.saveConf(simu.iconf++);

  simu.resetCloseList(simu.dVerlet);
  std::cout << "Beginning iterations." << std::endl;
  simu.integrate();

  return 0;
}