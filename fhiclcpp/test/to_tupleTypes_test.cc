// ======================================================================
//
// test of tuple-type decoding
//
// template<typename T,std::size_t SIZE>
// void decode(std::any const& a, std::array<T,SIZE>& result);
//
// template<typename KEY,typename VALUE>
// void decode(std::any const& a, std::pair<KEY,VALUE>& result);
//
// template<typename ... ARGS>
// void decode(std::any const& a, std::tuple<ARGS...>& result);
//
// ======================================================================

#include "cetlib_except/demangle.h"
#include "fhiclcpp/ParameterSet.h"

#include <array>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>
#include <typeinfo>
#include <utility>
#include <vector>

using namespace fhicl;

namespace {
  template <typename T>
  void
  print(T const& t)
  {
    std::cout << std::setw(15) << std::left
              << cet::demangle_symbol(typeid(t).name()) << " with value \"" << t
              << "\"\n";
  }
}

int
main()
{
  cet::filepath_lookup policy("FHICL_FILE_PATH");
  std::string const in("to_tupleTypes_test.fcl");
  auto const pset = ParameterSet::make(in, policy);

  //======================================================================
  // array checking
  //======================================================================

  std::cout << '\n'
            << "=================================\n"
            << " ARRAY CHECK\n"
            << "=================================\n\n";

  // warmup
  auto const vs0 = pset.get<std::vector<std::string>>("vec0");

  // to array
  auto const array = pset.get<std::array<std::string, 4>>("vec0");
  auto const vec_arrays =
    pset.get<std::vector<std::array<std::string, 3>>>("vec_arrays");

  // test error
  try {
    auto const err0 = pset.get<std::array<std::string, 3>>("vec0");
  }
  catch (fhicl::exception const& e) {
    std::cout << e.what() << std::endl;
  }

  //======================================================================
  // pair checking
  //======================================================================

  std::cout << '\n'
            << "=================================\n"
            << " PAIR CHECK\n"
            << "=================================\n\n";

  // warmup
  auto const vs1 = pset.get<std::vector<std::string>>("vec1");
  auto const pr_as_strings = pset.get<std::vector<std::string>>("pair1");

  // to pair
  auto const pr = pset.get<std::pair<unsigned, std::string>>("pair1");

  // to vector of pairs
  auto const vofp =
    pset.get<std::vector<std::pair<std::string, unsigned>>>("pair2");
  std::cout << std::endl;
  for (auto const& entry : vofp) {
    std::cout << entry.first << " " << entry.second << std::endl;
  }

  // to pair of pairs
  auto const pofp =
    pset.get<std::pair<std::pair<std::string, unsigned>,
                       std::pair<std::string, unsigned>>>("pair2");
  std::cout << std::endl;
  std::cout << pofp.first.first << " " << pofp.first.second << std::endl
            << pofp.second.first << " " << pofp.second.second << std::endl;

  // test error
  try {
    auto const err1 = pset.get<std::pair<std::string, std::string>>("pair3");
  }
  catch (fhicl::exception const& e) {
    std::cout << e.what() << std::endl;
  }

  //======================================================================
  // tuple checking
  //======================================================================

  std::cout << '\n'
            << "=================================\n"
            << " TUPLE CHECK\n"
            << "=================================\n\n";

  // warmup
  auto const vs2 = pset.get<std::vector<std::string>>("vec2");
  auto const tup_as_strings = pset.get<std::vector<std::string>>("tup1");

  // to tuple
  auto const tup1 = pset.get<std::tuple<int, std::string, double>>("tup1");

  print(std::get<0>(tup1));
  print(std::get<1>(tup1));
  print(std::get<2>(tup1));

  // vector of tuples
  auto const composers =
    pset
      .get<std::vector<std::tuple<std::size_t, std::string, unsigned, double>>>(
        "tup2");

  for (auto const& composer : composers) {
    std::cout << std::endl;
    print(std::get<0>(composer));
    print(std::get<1>(composer));
    print(std::get<2>(composer));
    print(std::get<3>(composer));
  }

  // test error
  try {
    auto const err1 =
      pset.get<std::vector<std::tuple<std::size_t, std::string, unsigned>>>(
        "tup2");
  }
  catch (fhicl::exception const& e) {
    std::cout << e.what() << std::endl;
  }

  // tuple of tuples
  auto const factoids = pset.get<
    std::tuple<std::tuple<std::string, std::string, std::string, std::string>,
               std::tuple<std::string, std::string, std::string, std::string>,
               std::tuple<std::string,
                          std::string,
                          std::string,
                          std::string,
                          unsigned,
                          unsigned,
                          unsigned>>>("tup3");

  auto const nine_symphonies = std::get<0>(factoids);
  std::cout << std::endl;
  print(std::get<0>(nine_symphonies));
  print(std::get<1>(nine_symphonies));
  print(std::get<2>(nine_symphonies));
  print(std::get<3>(nine_symphonies));

  auto const less_than_9_symphonies = std::get<1>(factoids);
  std::cout << std::endl;
  print(std::get<0>(less_than_9_symphonies));
  print(std::get<1>(less_than_9_symphonies));
  print(std::get<2>(less_than_9_symphonies));

  auto const piano_sonatas = std::get<2>(factoids);
  std::cout << std::endl;
  print(std::get<0>(piano_sonatas));
  print(std::get<1>(piano_sonatas));
  print(std::get<2>(piano_sonatas));
  print(std::get<3>(piano_sonatas));
  print(std::get<4>(piano_sonatas));
  print(std::get<5>(piano_sonatas));
  print(std::get<6>(piano_sonatas));
}
