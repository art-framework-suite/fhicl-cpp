#define BOOST_TEST_MODULE (ParameterSet test)

#include "boost/test/unit_test.hpp"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/test/boost_test_print_pset.h"

#include <cstddef>
#include <string>
#include <vector>

using namespace fhicl;

struct SampleConfigFixture {
  SampleConfigFixture();

  ParameterSet pset;
};

SampleConfigFixture::SampleConfigFixture()
{
  cet::filepath_lookup policy("FHICL_FILE_PATH");
  std::string cfg_in("Sample.cfg");
  pset = ParameterSet::make(cfg_in, policy);
}

BOOST_FIXTURE_TEST_SUITE(sampleConfig, SampleConfigFixture)

BOOST_AUTO_TEST_CASE(Local)
{
  fhicl::ParameterSet j;
  j.put("y", -1);
  auto const orig = pset.get<fhicl::ParameterSet>("j");
  BOOST_TEST(j == orig);
  BOOST_TEST(orig.get<int>("y") == -1);
  BOOST_TEST(pset.get<std::vector<int>>("m")[0] == -1);
  for (auto n : pset.get_names())
    BOOST_TEST("x" != n);
}

BOOST_AUTO_TEST_CASE(DeepInjection)
{
  fhicl::ParameterSet l;
  l.put("zz", -2);
  fhicl::ParameterSet k;
  k.put("l", l);
  auto const orig = pset.get<fhicl::ParameterSet>("k");
  BOOST_TEST(k == orig);
  BOOST_TEST(orig.get<fhicl::ParameterSet>("l").get<int>("zz") == -2);
  BOOST_TEST(orig.get<int>("l.zz") == -2);
}

BOOST_AUTO_TEST_CASE(DoubleStringMismatchDefaulted)
{
  std::string s;
  BOOST_TEST(pset.get_if_present("e", s));
  BOOST_TEST(s == "rain");

  try {
    pset.get<double>("e", 2.0);
    BOOST_FAIL("Failed to throw an exception as expected");
  }
  catch (fhicl::exception& e) {
    BOOST_TEST(e.categoryCode() == type_mismatch);
  }
  catch (...) {
    BOOST_FAIL("Wrong exception type thrown");
  }
}

BOOST_AUTO_TEST_CASE(DoubleStringMismatchNoDefault)
{
  try {
    pset.get<double>("e");
    BOOST_FAIL("Failed to throw an exception as expected");
  }
  catch (fhicl::exception& e) {
    BOOST_TEST(e.categoryCode() == type_mismatch);
  }
  catch (...) {
    BOOST_FAIL("Wrong exception type thrown");
  }
}

BOOST_AUTO_TEST_CASE(StringSuccess)
{
  BOOST_TEST("rain" == pset.get<std::string>("e"));
}

BOOST_AUTO_TEST_CASE(NotFound)
{
  try {
    pset.get<double>("zz");
    BOOST_FAIL("Failed to throw an exception as expected");
  }
  catch (fhicl::exception& e) {
    BOOST_TEST(e.categoryCode() == cant_find);
  }
  catch (...) {
    BOOST_FAIL("Wrong exception type thrown");
  }
}

BOOST_AUTO_TEST_CASE(DeepVector)
{
  using vv_t = std::vector<std::vector<uint32_t>>;
  vv_t vv;
  BOOST_CHECK_NO_THROW(vv = pset.get<vv_t>("vv"));
  BOOST_TEST(vv.front().back() == 3u);
  BOOST_TEST(vv.back().front() == 2u);
  BOOST_TEST(vv.back().back() == 4u);
}

BOOST_AUTO_TEST_CASE(is_key_to)
{
  // p1: {
  //    p2: {
  //       i: 37
  //       js: [3,2,67]
  //       t:{}
  //    }
  // }

  int const i{37};
  std::vector<int> const js{3, 2, 67};
  fhicl::ParameterSet const t{};
  fhicl::ParameterSet p;
  p.put("i", i);
  p.put("js", js);
  p.put("t", t);
  fhicl::ParameterSet ps;
  ps.put("p", p);

  BOOST_TEST(ps.is_key_to_atom("p.i"));
  BOOST_TEST(ps.is_key_to_sequence("p.js"));
  BOOST_TEST(ps.is_key_to_atom("p.js[2]"));
  BOOST_TEST(ps.is_key_to_table("p.t"));
}

BOOST_AUTO_TEST_CASE(put)
{
  std::string const sval = "friendly";
  std::string const sval2 = "unfriendly";
  BOOST_CHECK_NO_THROW(pset.put("putTest", sval));
  BOOST_TEST(pset.get<std::string>("putTest") == sval);
  BOOST_CHECK_EXCEPTION(
    pset.put("putTest", sval2), fhicl::exception, [](auto const& e) {
      return e.categoryCode() == fhicl::error::cant_insert;
    });
}

BOOST_AUTO_TEST_CASE(put_or_replace)
{
  std::string const sval = "friendly";
  std::string const sval2 = "superfriendly";
  BOOST_CHECK_NO_THROW(pset.put_or_replace("putOrReplaceTest", sval));
  BOOST_TEST(pset.get<std::string>("putOrReplaceTest") == sval);
  BOOST_CHECK_NO_THROW(pset.put_or_replace("putOrReplaceTest", sval2));
  BOOST_TEST(pset.get<std::string>("putOrReplaceTest") == sval2);
}

BOOST_AUTO_TEST_CASE(put_or_replace_compatible_nil)
{
  std::string const sval = "friendly";
  std::vector<int> vval{1, 2, 3, 5, 7, 11, 13};
  fhicl::ParameterSet psval;
  psval.put("junk", 3);
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace("putOrReplaceCompatibleTest")); // Nil.
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", sval));
  BOOST_TEST(pset.get<std::string>("putOrReplaceCompatibleTest") == sval);
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace("putOrReplaceCompatibleTest")); // Nil.
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", vval.front()));
  BOOST_TEST(pset.get<int>("putOrReplaceCompatibleTest") == vval.front());
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace("putOrReplaceCompatibleTest")); // Nil.
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", vval));
  BOOST_TEST(pset.get<decltype(vval)>("putOrReplaceCompatibleTest") == vval);
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace("putOrReplaceCompatibleTest")); // Nil.
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", psval));
  BOOST_TEST(pset.get<decltype(psval)>("putOrReplaceCompatibleTest") == psval);
}

BOOST_AUTO_TEST_CASE(put_or_replace_compatible_atom)
{
  std::string const sval = "friendly";
  std::string const sval2 = "superfriendly";
  std::vector<int> vval{1, 2, 3, 5, 7, 11, 13};
  fhicl::ParameterSet psval;
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace("putOrReplaceCompatibleTest")); // Nil.
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", sval));
  BOOST_TEST(pset.get<std::string>("putOrReplaceCompatibleTest") == sval);
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", sval2));
  BOOST_TEST(pset.get<std::string>("putOrReplaceCompatibleTest") == sval2);
  BOOST_CHECK_EXCEPTION(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", vval),
    fhicl::exception,
    [](auto const& e) {
      return e.categoryCode() == fhicl::error::cant_insert;
    });
  BOOST_CHECK_EXCEPTION(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", psval),
    fhicl::exception,
    [](auto const& e) {
      return e.categoryCode() == fhicl::error::cant_insert;
    });
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", vval.front()));
  BOOST_TEST(pset.get<int>("putOrReplaceCompatibleTest") == vval.front());
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace("putOrReplaceCompatibleTest")); // Nil.
}

BOOST_AUTO_TEST_CASE(put_or_replace_compatible_sequence)
{
  std::string const sval = "friendly";
  std::vector<int> vval{1, 2, 3, 5, 7, 11, 13};
  std::vector<int> vval2{4, 6, 8, 9, 10, 12, 14};
  fhicl::ParameterSet psval;
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace("putOrReplaceCompatibleTest")); // Nil.
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", vval));
  BOOST_TEST(pset.get<decltype(vval)>("putOrReplaceCompatibleTest") == vval);
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", vval2));
  BOOST_TEST(pset.get<decltype(vval)>("putOrReplaceCompatibleTest") == vval2);
  BOOST_CHECK_EXCEPTION(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", sval),
    fhicl::exception,
    [](auto const& e) {
      return e.categoryCode() == fhicl::error::cant_insert;
    });
  BOOST_CHECK_EXCEPTION(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", psval),
    fhicl::exception,
    [](auto const& e) {
      return e.categoryCode() == fhicl::error::cant_insert;
    });
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace("putOrReplaceCompatibleTest")); // Nil.
}

BOOST_AUTO_TEST_CASE(put_or_replace_compatible_table)
{
  std::string const sval = "friendly";
  std::vector<int> vval{1, 2, 3, 5, 7, 11, 13};
  fhicl::ParameterSet psval, psval2;
  psval.put("junk", 3);
  psval.put("ethel", 3);
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace("putOrReplaceCompatibleTest")); // Nil.
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", psval));
  BOOST_TEST(pset.get<decltype(psval)>("putOrReplaceCompatibleTest") == psval);
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", psval2));
  BOOST_TEST(pset.get<decltype(psval)>("putOrReplaceCompatibleTest") == psval2);
  BOOST_CHECK_EXCEPTION(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", sval),
    fhicl::exception,
    [](auto const& e) {
      return e.categoryCode() == fhicl::error::cant_insert;
    });
  BOOST_CHECK_EXCEPTION(
    pset.put_or_replace_compatible("putOrReplaceCompatibleTest", vval),
    fhicl::exception,
    [](auto const& e) {
      return e.categoryCode() == fhicl::error::cant_insert;
    });
  BOOST_CHECK_NO_THROW(
    pset.put_or_replace("putOrReplaceCompatibleTest")); // Nil.
}

unsigned
ctox(char c)
{
  switch (c) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    return c - '0';
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
    return 10 + c - 'a';
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
    return 10 + c - 'A';
  default:
    throw std::string("ctox(): invalid hex character");
  }
}

unsigned
hex(std::string const& from)
{
  if (from.size() < 3 || from[0] != '0' || (from[1] != 'x' && from[1] != 'X'))
    throw std::string("hex(): invalid argument");

  unsigned result = 0u;
  for (std::string::const_iterator it = from.begin() + 2, e = from.end();
       it != e;
       ++it)
    result = 16u * result + ctox(*it);
  return result;
}

BOOST_AUTO_TEST_CASE(Custom)
{
  BOOST_TEST(pset.get<std::string>("n") == "0x123");
  unsigned u{};
  BOOST_TEST(pset.get_if_present("n", u, hex));
  BOOST_TEST(pset.get<unsigned>("n", hex) == u);
  BOOST_TEST(pset.get<unsigned>("n", hex), (((1u) * 16u + 2u) * 16u + 3u));
  BOOST_CHECK_THROW(pset.get_if_present("e", u, hex), std::string);
}

BOOST_AUTO_TEST_SUITE_END()
