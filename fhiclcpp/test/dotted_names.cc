#define BOOST_TEST_MODULE (has_key of dotted names test)
#include "boost/test/unit_test.hpp"

#include "fhiclcpp/ParameterSet.h"

#include <string>

BOOST_AUTO_TEST_SUITE(dotted_names_test)

BOOST_AUTO_TEST_CASE(dotted_names_t)
{
  std::string text("m1: { val: 3 name: boo} ");
  auto const ps = fhicl::ParameterSet::make(text);

  BOOST_TEST(ps.has_key("m1.val"));
  BOOST_TEST(!ps.has_key("m1.val.three"));
  BOOST_TEST(ps.get<int>("m1.val") == 3);
  BOOST_TEST(ps.get<std::string>("m1.name") == "boo");
  BOOST_TEST(!ps.has_key("m0"));
  BOOST_TEST(!ps.has_key("m0.nothing.to.see.here"));
}

BOOST_AUTO_TEST_SUITE_END()
