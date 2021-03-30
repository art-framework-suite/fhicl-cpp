#include "fhiclcpp/ParameterSet.h"

#include <cassert>
#include <string>

int
main()
{
  cet::filepath_lookup policy("FHICL_FILE_PATH");
  std::string in("Sample.cfg");

  auto const pset1 = fhicl::ParameterSet::make(in, policy);
  auto const pset2 = fhicl::ParameterSet::make(pset1.to_string());

  assert(pset1 == pset2);

  assert(pset2.get<int>("a") == 1);
  assert(pset2.get<unsigned int>("a") == 1);
  assert(pset2.get<int>("b") == -1);
  //  assert(pset2.get<unsigned int>("b", 0) == 0);

  assert(pset2.get<double>("c") == 0.0);
  assert(pset2.get<double>("d") == 0.5);

  assert(pset2.get<std::string>("e").compare("rain") == 0);
  assert(pset2.get<std::string>("f").compare("32") == 0);
  assert(pset2.get<int>("f") == 32);

  assert(pset2.get<std::vector<int>>("g")[2] == 3);

  assert(pset2.get<std::vector<fhicl::ParameterSet>>("h")[1]
           .get<std::string>("h2")
           .compare("h2") == 0);

  assert(pset2.get<fhicl::ParameterSet>("i")
           .get<fhicl::ParameterSet>("i1")
           .get<std::string>("i1_1")
           .compare("test") == 0);

  return 0;
} // main()
