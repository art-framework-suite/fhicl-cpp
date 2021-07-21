// ======================================================================
//
// test ParameterSet::to_indented_string()
//
// ======================================================================

#define BOOST_TEST_MODULE (to_indented_string test)

#include "boost/test/unit_test.hpp"
#include "fhiclcpp/ParameterSet.h"

#include <numeric>
#include <string>
#include <vector>

using namespace fhicl;
using namespace std;

using ints = std::vector<int>;
using nested_ints = std::vector<ints>;

namespace {
  auto to_ind_str = [](auto const& pset) { return pset.to_indented_string(); };
}

BOOST_AUTO_TEST_SUITE(values_test)

BOOST_AUTO_TEST_CASE(atoms)
{
  ParameterSet pset;
  BOOST_TEST(pset.is_empty());
  BOOST_TEST(to_ind_str(pset) == "");

  pset.put<std::string>("a", "string1");
  BOOST_TEST(to_ind_str(pset) == "a: \"string1\"\n");

  pset.put<int>("b", -1234);
  BOOST_TEST(to_ind_str(pset) == "a: \"string1\"\n"
                                 "b: -1234\n");

  pset.put<bool>("c", false);
  BOOST_TEST(to_ind_str(pset) == "a: \"string1\"\n"
                                 "b: -1234\n"
                                 "c: false\n");
}

BOOST_AUTO_TEST_CASE(sequences)
{
  ParameterSet pset;
  pset.put<ints>("a", {});
  BOOST_TEST(to_ind_str(pset) == "a: []\n");

  pset.put<ints>("b", {11});
  BOOST_TEST(to_ind_str(pset) == "a: []\n"
                                 "b: [\n"
                                 "   11\n"
                                 "]\n");

  pset.put<ints>("c", {11, 12, 13});
  BOOST_TEST(to_ind_str(pset) == "a: []\n"
                                 "b: [\n"
                                 "   11\n"
                                 "]\n"
                                 "c: [\n"
                                 "   11,\n"
                                 "   12,\n"
                                 "   13\n"
                                 "]\n");
}

BOOST_AUTO_TEST_CASE(tables)
{
  ParameterSet pset;

  ParameterSet p;
  pset.put<ParameterSet>("p1", p);
  BOOST_TEST(to_ind_str(pset) == "p1: {}\n");

  p.put<std::string>("a", "string1");
  pset.put<ParameterSet>("p2", p);
  BOOST_TEST(to_ind_str(pset) == "p1: {}\n"
                                 "p2: {\n"
                                 "   a: \"string1\"\n"
                                 "}\n");

  p.put<int>("b", -1234);
  pset.put<ParameterSet>("p3", p);
  BOOST_TEST(to_ind_str(pset) == "p1: {}\n"
                                 "p2: {\n"
                                 "   a: \"string1\"\n"
                                 "}\n"
                                 "p3: {\n"
                                 "   a: \"string1\"\n"
                                 "   b: -1234\n"
                                 "}\n");

  p.put<bool>("c", false);
  pset.put<ParameterSet>("p4", p);
  BOOST_TEST(to_ind_str(pset) == "p1: {}\n"
                                 "p2: {\n"
                                 "   a: \"string1\"\n"
                                 "}\n"
                                 "p3: {\n"
                                 "   a: \"string1\"\n"
                                 "   b: -1234\n"
                                 "}\n"
                                 "p4: {\n"
                                 "   a: \"string1\"\n"
                                 "   b: -1234\n"
                                 "   c: false\n"
                                 "}\n");
}

BOOST_AUTO_TEST_CASE(combo)
{
  ParameterSet p;
  p.put<ints>("v", {11, 12, 13});

  ParameterSet pset;
  pset.put<ParameterSet>("p", p);
  BOOST_TEST(to_ind_str(pset) == "p: {\n"
                                 "   v: [\n"
                                 "      11,\n"
                                 "      12,\n"
                                 "      13\n"
                                 "   ]\n"
                                 "}\n");
}

BOOST_AUTO_TEST_CASE(sequence_printout)
{
  ints v(20, 0);
  std::iota(v.begin(), v.end(), 1);

  ParameterSet p;
  p.put<ints>("v", v);

  ParameterSet pset;
  pset.put<ParameterSet>("p", p);
  BOOST_TEST(to_ind_str(pset) == "p: {\n"
                                 "   v: [\n"
                                 "      1,\n"
                                 "      2,\n"
                                 "      3,\n"
                                 "      4,\n"
                                 "      5,\n"
                                 "      6,\n"
                                 "      7,\n"
                                 "      8,\n"
                                 "      9,\n"
                                 "      10,\n"
                                 "      11,\n"
                                 "      12,\n"
                                 "      13,\n"
                                 "      14,\n"
                                 "      15,\n"
                                 "      16,\n"
                                 "      17,\n"
                                 "      18,\n"
                                 "      19,\n"
                                 "      20\n"
                                 "   ]\n"
                                 "}\n");
}

BOOST_AUTO_TEST_CASE(nested_sequence_printout_empty)
{
  nested_ints nested;
  ParameterSet p;
  p.put<nested_ints>("empty", nested);

  ParameterSet pset;
  pset.put<ParameterSet>("p", p);
  BOOST_TEST(to_ind_str(pset) == "p: {\n"
                                 "   empty: []\n"
                                 "}\n");
}

BOOST_AUTO_TEST_CASE(nested_sequence_printout)
{
  ints v(4, 0);
  std::iota(v.begin(), v.end(), 1);
  nested_ints nested{v};

  v.push_back(5);
  nested.push_back(v);
  ParameterSet p;
  p.put<nested_ints>("nested", nested);

  ParameterSet pset;
  pset.put<ParameterSet>("p", p);
  BOOST_TEST(to_ind_str(pset) == "p: {\n"
                                 "   nested: [\n"
                                 "      [\n"
                                 "         1,\n"
                                 "         2,\n"
                                 "         3,\n"
                                 "         4\n"
                                 "      ],\n"
                                 "      [\n"
                                 "         1,\n"
                                 "         2,\n"
                                 "         3,\n"
                                 "         4,\n"
                                 "         5\n"
                                 "      ]\n"
                                 "   ]\n"
                                 "}\n");
}
BOOST_AUTO_TEST_SUITE_END()
