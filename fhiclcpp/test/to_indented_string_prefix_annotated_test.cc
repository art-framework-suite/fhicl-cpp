// ======================================================================
//
// test of to_indented_string with prefixed annotations
//
// ======================================================================

#include "fhiclcpp/ParameterSet.h"

#include <iostream>
#include <string>

int
main()
{

  // Configuration file lookup policy.
  char const* fhicl_env = getenv("FHICL_FILE_PATH");
  std::string search_path;
  if (fhicl_env == nullptr) {
    std::cerr << "Expected environment variable FHICL_FILE_PATH is "
              << "missing or empty: using \".\"\n";
    search_path = ".:";
  } else {
    search_path = std::string(fhicl_env) + ":";
  }

  cet::filepath_lookup policy(search_path);
  std::string const in{"to_indented_string_annotated_test.fcl"};

  auto pset = fhicl::ParameterSet::make(in, policy);

  pset.put<int>("another_number", 4);
  pset.put_or_replace<int>("some_number", 8);
  pset.put_or_replace<double>("double", 234.6);

  std::cout << pset.to_indented_string(
                 0, fhicl::detail::print_mode::prefix_annotated)
            << std::endl;
}
