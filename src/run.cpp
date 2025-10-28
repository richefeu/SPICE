#include "run.hpp"

int main(int argc, char const *argv[]) {
  SPICE simu;
  simu.head();

  if (argc != 2) {
    std::cout << "usage: run <input-conf-file>" << std::endl;
    return 0;
  } else {
    if (fileTool::containsOnlyDigits(argv[1])) {
      int num = std::atoi(argv[1]);
      std::cout << SPICE_INFO << "load conf" << num << std::endl;
			simu.loadConf(num);
    } else {
			std::cout << SPICE_INFO << "load " << argv[1] << std::endl;
      simu.loadConf(argv[1]);
    }
  }

  simu.resetCloseList(simu.dVerlet);
  simu.integrate();

  return 0;
}