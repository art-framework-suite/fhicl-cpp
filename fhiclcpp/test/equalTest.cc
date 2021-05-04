#define BOOST_TEST_MODULE (equality test)

#include "boost/test/unit_test.hpp"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/test/boost_test_print_pset.h"

using namespace fhicl;

BOOST_AUTO_TEST_SUITE(document_test)

BOOST_AUTO_TEST_CASE(leading_zeros)
{
  auto const pset1 = ParameterSet::make("x : 5");
  auto const pset2 = ParameterSet::make("x : 005");
  BOOST_TEST(pset1 == pset2);
  BOOST_TEST(pset1.get<int>("x") == pset2.get<int>("x"));
}

BOOST_AUTO_TEST_SUITE_END()
