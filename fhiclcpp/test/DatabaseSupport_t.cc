#define BOOST_TEST_MODULE (database test)

#include "boost/test/unit_test.hpp"

#include <string>
#include <vector>

#include "fhiclcpp/DatabaseSupport.h"
#include "fhiclcpp/ParameterSet.h"

BOOST_AUTO_TEST_SUITE(database_support_test)

BOOST_AUTO_TEST_CASE(no_nesting)
{
  std::vector<std::string> records;
  std::vector<std::string> hashes;

  BOOST_CHECK_NO_THROW(fhicl::decompose_fhicl("db_0.fcl", records, hashes));
  BOOST_TEST(records.size() == hashes.size());
  BOOST_TEST(records.size() == 1ul);
}

BOOST_AUTO_TEST_CASE(two_nested)
{
  std::vector<std::string> records;
  std::vector<std::string> hashes;

  cet::filepath_maker fpm;
  auto const p = fhicl::ParameterSet::make("db_1.fcl", fpm);
  std::cout << "\n";
  std::cout << "Compact string:\n";
  std::cout << p.to_compact_string() << std::endl;
  std::cout << "Non compact string:\n";
  std::cout << p.to_string() << std::endl;

  BOOST_CHECK_NO_THROW(fhicl::decompose_fhicl("db_1.fcl", records, hashes));
  BOOST_TEST(records.size() == hashes.size());
  BOOST_TEST(records.size() == 3ul);
}

BOOST_AUTO_TEST_CASE(nested_vector)
{
  std::vector<std::string> records;
  std::vector<std::string> hashes;

  cet::filepath_maker fpm;
  auto const p = fhicl::ParameterSet::make("db_2.fcl", fpm);
  auto const v = p.get<std::vector<fhicl::ParameterSet>>("a");
  std::cout << "\n";
  std::cout << p.to_compact_string() << std::endl;
  BOOST_TEST(v.size() == 2ul);

  BOOST_CHECK_NO_THROW(fhicl::decompose_fhicl("db_2.fcl", records, hashes));
  BOOST_TEST(records.size() == hashes.size());
  BOOST_TEST(records.size() == 3ul);
}

BOOST_AUTO_TEST_SUITE_END()
