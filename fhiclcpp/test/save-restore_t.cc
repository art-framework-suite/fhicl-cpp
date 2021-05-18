#include "cetlib/filepath_maker.h"
#include "fhiclcpp/ParameterSet.h"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

using namespace fhicl;
using namespace std;

int
main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "ERROR: expect exactly one filename!\n";
    exit(1);
  }
  ParameterSet pset_ref;
  cet::filepath_lookup policy(".:");
  try {
    pset_ref = ParameterSet::make(argv[1], policy);
  }
  catch (std::exception& e) {
    std::cerr << "ERROR: unable to parse FHiCL file "
              << "\"" << argv[1] << "\"\n"
              << e.what();
    exit(1);
  }

  string const pset_string(pset_ref.to_string());
  auto const pset_test = ParameterSet::make(pset_string);

  string pset_test_string(pset_test.to_string());
  if (pset_string != pset_test_string) {
    std::cerr << "ERROR: \n"
              << pset_string << "\n"
              << " !=\n"
              << pset_test_string << "\n";
    exit(1);
  }
}
