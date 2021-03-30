#define BOOST_TEST_MODULE (document test)

#include "boost/test/unit_test.hpp"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/test/boost_test_print_pset.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace fhicl;
using namespace std;

void
readFile(string fileName, string& content)
{
  ifstream in(fileName.c_str());
  for (string line; getline(in, line);)
    content.append(line).append("\n");
}

BOOST_AUTO_TEST_SUITE(document_test)

BOOST_AUTO_TEST_CASE(doc)
{
  cet::filepath_lookup_nonabsolute policy("FHICL_FILE_PATH");
  auto const ps1 = ParameterSet::make(
    boost::unit_test::framework::master_test_suite().argv[1], policy);
  auto const ps2 = ParameterSet::make(ps1.to_string());
  BOOST_TEST(ps1 == ps2);

  // Alternative representation.
  auto const ps3 = ParameterSet::make(ps1.to_compact_string());
  BOOST_TEST(ps1 == ps3);
}

BOOST_AUTO_TEST_SUITE_END()
