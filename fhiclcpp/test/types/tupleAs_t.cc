// ======================================================================
//
// test tupleAs
//
// ======================================================================

#define BOOST_TEST_MODULE (tupleAs test)

#include "boost/test/unit_test.hpp"

#include "FixtureBase.h"
#include "fhiclcpp/types/OptionalTupleAs.h"
#include "fhiclcpp/types/Sequence.h"
#include "fhiclcpp/types/TupleAs.h"

#include <string>
#include <vector>

using namespace fhicl;
using namespace std;

namespace {

  struct ThreeNumbers {
    int i_, j_, k_;
    ThreeNumbers(int i, int j, int k) : i_{i}, j_{j}, k_{k} {}
  };

  struct Person {

    string name_;
    unsigned age_{};

    Person() = default;
    Person(string name, unsigned age) : name_{name}, age_{age} {}

    bool
    operator==(Person const& p) const
    {
      return (name_ == p.name_) && (age_ == p.age_);
    }

    bool
    operator!=(Person const& p) const
    {
      return !operator==(p);
    }
  };

  ostream&
  operator<<(ostream& os, Person const& p)
  {
    return os << "Name: " << p.name_ << "  Age: " << p.age_;
  }

  struct ToVector {

    ToVector(int j, vector<int> const& v)
    {
      for (auto number : v)
        value_.push_back(j * number);
    }

    vector<int> value_;
  };

  struct Config {
    TupleAs<ThreeNumbers(int, int, int)> tn1{Name("numbers1"),
                                             Comment("Vector for Geant")};
    TupleAs<ThreeNumbers(int, int, int)> tn2{Name("numbers2"),
                                             ThreeNumbers{1, 3, 5}};
    OptionalTupleAs<Person(string, unsigned)> person1{Name("person1")};
    TupleAs<Person(string, unsigned)> person2{Name("person2"),
                                              Person{"Jon", 97}};
    TupleAs<ToVector(int, Sequence<int>)> toVector{Name("toVector")};
    Sequence<TupleAs<Person(string, unsigned)>> people{Name("people")};
    Sequence<TupleAs<Person(string, unsigned)>> kids{
      Name("kids"),
      std::vector<Person>{{"Billy", 10}, {"Susie", 14}}};
  };

  struct Fixture : fhiclcpp_types::FixtureBase<Config> {
    Fixture() : FixtureBase("tupleAs_t.fcl") {}
  };
}
// provide use of 'Table<Config> config'

BOOST_FIXTURE_TEST_SUITE(tupleAs_test, Fixture)

BOOST_AUTO_TEST_CASE(tupleAs_simple)
{
  ThreeNumbers const& tn1 = config().tn1();
  BOOST_TEST(tn1.i_ == 1);
  BOOST_TEST(tn1.j_ == 9);
  BOOST_TEST(tn1.k_ == 17);
}

BOOST_AUTO_TEST_CASE(tupleAs_default_value)
{
  ThreeNumbers const& tn2 = config().tn2();
  BOOST_TEST(tn2.i_ == 1);
  BOOST_TEST(tn2.j_ == 3);
  BOOST_TEST(tn2.k_ == 5);
}

BOOST_AUTO_TEST_CASE(tupleAs_toVector)
{
  ToVector const& v = config().toVector();
  auto const ref = {5, 10, 15, 20};
  BOOST_TEST(v.value_ == ref, boost::test_tools::per_element{});
}

BOOST_AUTO_TEST_CASE(tupleAs_inSequence)
{
  auto const& people = config().people();
  auto const ref = {Person{"Alice", 1},
                    Person{"Bob", 2},
                    Person{"Charlie", 3},
                    Person{"Danielle", 4},
                    Person{"Edgar", 5}};
  BOOST_TEST(people == ref, boost::test_tools::per_element{});
}

BOOST_AUTO_TEST_CASE(tupleAs_inSequence_2)
{
  auto const& people = config().kids();
  auto const ref = {Person{"Billy", 10}, Person{"Susie", 14}};
  BOOST_TEST(people == ref, boost::test_tools::per_element{});
}

BOOST_AUTO_TEST_SUITE_END()
