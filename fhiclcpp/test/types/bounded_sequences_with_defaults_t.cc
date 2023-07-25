#define BOOST_TEST_MODULE (Bounded sequences with defaults)

#include "boost/test/unit_test.hpp"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/Sequence.h"
#include "fhiclcpp/types/Table.h"
#include "fhiclcpp/types/Tuple.h"
#include "fhiclcpp/types/detail/validationException.h"

#include <string>

using namespace fhicl;
using namespace std;
using namespace string_literals;

namespace {

  struct ArrayConfig {
    Sequence<string, 2> composers{Name{"composers"}, {"Mahler", "Elgar"}};
  };

  struct TupleConfig {
    Tuple<string, unsigned> ages{Name{"ages"}, {"David"s, 9}};
  };

  template <typename T>
  Table<T>
  validate(std::string const& cfg)
  {
    Table<T> validated_config{Name("validatedConfig")};
    validated_config.validate(ParameterSet::make(cfg));
    return validated_config;
  }
}

BOOST_AUTO_TEST_SUITE(bounded_sequence_with_defaults)

BOOST_AUTO_TEST_CASE(GoodArray)
{
  string const good{};
  auto const& validated = validate<ArrayConfig>(good);
  BOOST_TEST(validated().composers(0) == "Mahler"s);
  BOOST_TEST(validated().composers(1) == "Elgar"s);
}

BOOST_AUTO_TEST_CASE(GoodTuple)
{
  string const good{};
  auto const& validated = validate<TupleConfig>(good);
  BOOST_TEST(validated().ages.get<0>() == "David"s);
  BOOST_TEST(validated().ages.get<1>() == 9u);
}

/*BOOST_AUTO_TEST_CASE(BadSequence)
{
  string const bad{"composers: [Beethoven]"};
  // Because the 'composers' parameter is a bounded sequence of atomic
  // types, the preset_value function indirectly throws a
  // fhicl::exception instead of a fhicl::detail::validationException.
  // This should probably be fixed at some point.
  BOOST_REQUIRE_THROW(validate<ArrayConfig>(bad), fhicl::exception);
}*/

BOOST_AUTO_TEST_CASE(BadTuple)
{
  string const bad{"ages: [Jenny]"};
  BOOST_REQUIRE_THROW(validate<TupleConfig>(bad), detail::validationException);
}

BOOST_AUTO_TEST_SUITE_END()
