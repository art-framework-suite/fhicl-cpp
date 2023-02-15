#include "cetlib/filepath_maker.h"
#include "cetlib_except/exception.h"
#include "fhiclcpp/ParameterSet.h"

#include <iostream>

int
main()
try {
  std::string in("test_config_fail.fcl");
  cet::filepath_lookup policy("FHICL_FILE_PATH");
  fhicl::ParameterSet::make(in, policy);
  return 1;
}
catch (cet::exception const& e) {
  std::cerr << e.what() << '\n';
  return 0;
}
