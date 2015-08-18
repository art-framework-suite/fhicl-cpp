// ======================================================================
//
// test static_types keymap for tables

/* The purpose of this test is to verify that types 14-21 below create
   the correct key maps for ParameterSet validation.

   In what follows, ’T’ represents a type supported by an Atom<>
   and ’S’ represents an explicitly constructed struct that may
   contain Atom<>, Sequence<>, Tuple<>, or Table<> objects.

   [ 1] Atom<T>;
   [ 2] Sequence<T>
   [ 3] Sequence<T,SZ>
   [ 4] Tuple<T...>
   [ 5] Tuple< Sequence<T>, U...>
   [ 6] Tuple< Sequence<T,SZ>, U...>
   [ 7] Tuple< Tuple<T...>,U...>
   [ 8] Sequence< Tuple<T...> >
   [ 9] Sequence< Tuple<T...>, SZ >
   [10] Sequence< Sequence<T> >
   [11] Sequence< Sequence<T,SZ> >
   [12] Sequence< Sequence<T>, SZ >
   [13] Sequence< Sequence<T,SZ>, SZ >

   14-21 cannot support default arguments (since Table<> cannot have a default)

   [14] Table<S>
   [15] Sequence< Table<S> >
   [16] Sequence< Table<S>, SZ >
   [17] Tuple< Table<S>, U... >
   [18] Tuple< Sequence< Table<S> >, U... >
   [19] Tuple< Sequence< Table<S>, SZ>, U... >
   [20] Sequence< Tuple< Table<S>, U... > >
   [21] Sequence< Tuple< Table<S>, U... >, SZ>

 */

// ======================================================================

#define BOOST_TEST_MODULE ( keymap test with tables )

#include "boost/test/auto_unit_test.hpp"
#include "boost/test/test_tools.hpp"

#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/detail/ParameterReferenceRegistry.h"
#include "fhiclcpp/types/Sequence.h"
#include "fhiclcpp/types/Tuple.h"

#include <iostream>
#include <string>
#include <vector>

using namespace fhicl;
using namespace fhicl::detail;
using namespace std;

namespace {

  std::vector<std::string>
  key_map(ParameterBase const * pb)
  {
    auto keys = ParameterReferenceRegistry::get_parameter_keys(pb);
    ParameterReferenceRegistry::instance().clear();
    return keys;
  }

  struct S {
    Atom<int> test { Key("atom") };
    Sequence<int,2> seq { Key("sequence") };
    Tuple<int,double,bool> tuple { Key("tuple") };
  };

}

BOOST_AUTO_TEST_SUITE( static_types_keymap_test )

// [14] Table<S>
BOOST_AUTO_TEST_CASE( table_t )
{
  Table<S> test { Key("table") };
  auto map = key_map(&test);
  auto ref = {"table",
              "table.atom",
              "table.sequence",
              "table.sequence[0]",
              "table.sequence[1]",
              "table.tuple",
              "table.tuple[0]",
              "table.tuple[1]",
              "table.tuple[2]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [15] Sequence< Table<S> >
BOOST_AUTO_TEST_CASE( table_in_seq_t )
{
  Sequence< Table<S> > test { Key("seqtable") };
  auto map = key_map(&test);
  auto ref = {"seqtable",
              "seqtable[0]",
              "seqtable[0].atom",
              "seqtable[0].sequence",
              "seqtable[0].sequence[0]",
              "seqtable[0].sequence[1]",
              "seqtable[0].tuple",
              "seqtable[0].tuple[0]",
              "seqtable[0].tuple[1]",
              "seqtable[0].tuple[2]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [16] Sequence< Table<S>,2 >
BOOST_AUTO_TEST_CASE( table_in_seq_2_t )
{
  Sequence< Table<S>, 2 > test { Key("seqtable") };
  auto map = key_map(&test);
  auto ref = {"seqtable",
              "seqtable[0]",
              "seqtable[0].atom",
              "seqtable[0].sequence",
              "seqtable[0].sequence[0]",
              "seqtable[0].sequence[1]",
              "seqtable[0].tuple",
              "seqtable[0].tuple[0]",
              "seqtable[0].tuple[1]",
              "seqtable[0].tuple[2]",
              "seqtable[1]",
              "seqtable[1].atom",
              "seqtable[1].sequence",
              "seqtable[1].sequence[0]",
              "seqtable[1].sequence[1]",
              "seqtable[1].tuple",
              "seqtable[1].tuple[0]",
              "seqtable[1].tuple[1]",
              "seqtable[1].tuple[2]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [17] Tuple< Table<S>, U... >
BOOST_AUTO_TEST_CASE( table_in_tuple_t )
{
  Tuple< Table<S>, int, double > test { Key("tuptable") };
  auto map = key_map(&test);
  auto ref = {"tuptable",
              "tuptable[0]",
              "tuptable[0].atom",
              "tuptable[0].sequence",
              "tuptable[0].sequence[0]",
              "tuptable[0].sequence[1]",
              "tuptable[0].tuple",
              "tuptable[0].tuple[0]",
              "tuptable[0].tuple[1]",
              "tuptable[0].tuple[2]",
              "tuptable[1]",
              "tuptable[2]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [18] Tuple< Sequence< Table<S> >, U... >
BOOST_AUTO_TEST_CASE( seqtable_in_tuple_t )
{
  Tuple< Sequence< Table<S> >, int, double > test { Key("seqtuptable") };
  auto map = key_map(&test);
  auto ref = {"seqtuptable",
              "seqtuptable[0]",
              "seqtuptable[0][0]",
              "seqtuptable[0][0].atom",
              "seqtuptable[0][0].sequence",
              "seqtuptable[0][0].sequence[0]",
              "seqtuptable[0][0].sequence[1]",
              "seqtuptable[0][0].tuple",
              "seqtuptable[0][0].tuple[0]",
              "seqtuptable[0][0].tuple[1]",
              "seqtuptable[0][0].tuple[2]",
              "seqtuptable[1]",
              "seqtuptable[2]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [19] Tuple< Sequence< Table<S>, SZ >, U... >
BOOST_AUTO_TEST_CASE( seqtable_2_in_tuple_t )
{
  Tuple< Sequence< Table<S>, 2 >, int, double > test { Key("seqtuptable") };
  auto map = key_map(&test);
  auto ref = {"seqtuptable",
              "seqtuptable[0]",
              "seqtuptable[0][0]",
              "seqtuptable[0][0].atom",
              "seqtuptable[0][0].sequence",
              "seqtuptable[0][0].sequence[0]",
              "seqtuptable[0][0].sequence[1]",
              "seqtuptable[0][0].tuple",
              "seqtuptable[0][0].tuple[0]",
              "seqtuptable[0][0].tuple[1]",
              "seqtuptable[0][0].tuple[2]",
              "seqtuptable[0][1]",
              "seqtuptable[0][1].atom",
              "seqtuptable[0][1].sequence",
              "seqtuptable[0][1].sequence[0]",
              "seqtuptable[0][1].sequence[1]",
              "seqtuptable[0][1].tuple",
              "seqtuptable[0][1].tuple[0]",
              "seqtuptable[0][1].tuple[1]",
              "seqtuptable[0][1].tuple[2]",
              "seqtuptable[1]",
              "seqtuptable[2]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [20] Sequence< Tuple< Table<S>, U... > >
BOOST_AUTO_TEST_CASE( tuptable_in_seq_t )
{
  Sequence< Tuple< Table<S>, int, double > > test { Key("tupseqtable") };
  auto map = key_map(&test);
  auto ref = {"tupseqtable",
              "tupseqtable[0]",
              "tupseqtable[0][0]",
              "tupseqtable[0][0].atom",
              "tupseqtable[0][0].sequence",
              "tupseqtable[0][0].sequence[0]",
              "tupseqtable[0][0].sequence[1]",
              "tupseqtable[0][0].tuple",
              "tupseqtable[0][0].tuple[0]",
              "tupseqtable[0][0].tuple[1]",
              "tupseqtable[0][0].tuple[2]",
              "tupseqtable[0][1]",
              "tupseqtable[0][2]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [21] Sequence< Tuple< Table<S>, U... >, SZ >
BOOST_AUTO_TEST_CASE( tuptable_in_seq_2_t )
{
  Sequence< Tuple< Table<S>, int, double >, 2 > test { Key("tupseqtable") };
  auto map = key_map(&test);
  auto ref = {"tupseqtable",
              "tupseqtable[0]",
              "tupseqtable[0][0]",
              "tupseqtable[0][0].atom",
              "tupseqtable[0][0].sequence",
              "tupseqtable[0][0].sequence[0]",
              "tupseqtable[0][0].sequence[1]",
              "tupseqtable[0][0].tuple",
              "tupseqtable[0][0].tuple[0]",
              "tupseqtable[0][0].tuple[1]",
              "tupseqtable[0][0].tuple[2]",
              "tupseqtable[0][1]",
              "tupseqtable[0][2]",
              "tupseqtable[1]",
              "tupseqtable[1][0]",
              "tupseqtable[1][0].atom",
              "tupseqtable[1][0].sequence",
              "tupseqtable[1][0].sequence[0]",
              "tupseqtable[1][0].sequence[1]",
              "tupseqtable[1][0].tuple",
              "tupseqtable[1][0].tuple[0]",
              "tupseqtable[1][0].tuple[1]",
              "tupseqtable[1][0].tuple[2]",
              "tupseqtable[1][1]",
              "tupseqtable[1][2]" };
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

BOOST_AUTO_TEST_SUITE_END()