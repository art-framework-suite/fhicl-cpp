#include "cetlib/filepath_maker.h"
#include "fhiclcpp/ParameterSet.h"

#include <iostream>
#include <string>

using namespace fhicl;

int
main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "ERROR: expect exactly one argument.\n";
    exit(1);
  }

  cet::filepath_lookup policy("FHICL_FILE_PATH");

  std::string cfg_in(argv[1]);
  auto const pset = ParameterSet::make(cfg_in, policy);
  std::cout << pset.to_string() << std::endl;

  return 0;
}
