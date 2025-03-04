#ifndef fhiclcpp_coding_h
#define fhiclcpp_coding_h

// ======================================================================
//
// coding
//
// ======================================================================
//
// Notes:
//
// exception handling
//
// - For std::pair- and std::tuple- supported decode functions,
//   exceptions are thrown using std::exception-derived classes since
//   fhicl::ParameterSet::get_one_ rethrows whatever exception it
//   receives.  If a fhicl::exception is rethrown with a
//   fhicl::exception, then the cetlib::exception class includes two
//   prolog/epilog statements instead of one.
//
// ======================================================================

#include "boost/lexical_cast.hpp"
#include "boost/numeric/conversion/cast.hpp"
#include "fhiclcpp/ParameterSetID.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/fwd.h"
#include "fhiclcpp/parse.h"
#include "fhiclcpp/type_traits.h"

#include <any>
#include <array>
#include <complex>
#include <concepts>
#include <cstdint>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace fhicl::detail {
  using ps_atom_t = std::string;
  using ps_sequence_t = std::vector<std::any>;
  using ldbl = long double;

  inline bool
  is_sequence(std::any const& val)
  {
    return val.type() == typeid(ps_sequence_t);
  }

  inline bool
  is_table(std::any const& val)
  {
    return val.type() == typeid(ParameterSetID);
  }

  bool is_nil(std::any const& val);

  // ----------------------------------------------------------------------

  ps_atom_t encode(std::string const&);       // string (w/ quotes)
  ps_atom_t encode(char const*);              // string (w/ quotes)
  ps_atom_t encode(std::nullptr_t);           // nil
  ps_atom_t encode(bool);                     // bool
  ParameterSetID encode(ParameterSet const&); // table
  ps_atom_t encode(std::uintmax_t);           // unsigned
  template <std::unsigned_integral T>
  ps_atom_t encode(T const&);      // unsigned
  ps_atom_t encode(std::intmax_t); // signed
  template <std::signed_integral T>
  ps_atom_t encode(T const&); // signed
  ps_atom_t encode(ldbl);     // floating-point
  template <std::floating_point T>
  ps_atom_t encode(T const&); // floating-point
  template <class T>
  ps_atom_t encode(std::complex<T> const&); // complex
  template <class T>
  ps_sequence_t encode(std::vector<T> const&); // sequence
  template <non_numeric T>
  std::string encode(T const&); // none of the above

  // ----------------------------------------------------------------------

  void decode(std::any const&, std::string&);    // string
  void decode(std::any const&, std::nullptr_t&); // nil
  void decode(std::any const&, bool&);           // bool
  void decode(std::any const&, ParameterSet&);   // table
  void decode(std::any const&, std::uintmax_t&); // unsigned

  template <std::unsigned_integral T>
  void decode(std::any const&, T&); // unsigned

  void decode(std::any const&, std::intmax_t&); // signed

  template <std::signed_integral T>
  void decode(std::any const&, T&); // signed

  void decode(std::any const&, ldbl&); // floating-point

  template <std::floating_point T>
  void decode(std::any const&, T&); // floating-point

  void decode(std::any const&, std::complex<ldbl>&); // complex

  template <class T>
  void decode(std::any const&, std::complex<T>&); // complex

  template <class T>
  void decode(std::any const&, std::vector<T>&); // sequence

  template <typename U>
  void decode_tuple(std::any const&, U& tuple); // tuple-type decoding

  template <typename T, std::size_t SIZE>
  void
  decode(std::any const& a, std::array<T, SIZE>& result) // std::array
  {
    decode_tuple(a, result);
  }

  template <typename KEY, typename VALUE>
  void
  decode(std::any const& a, std::pair<KEY, VALUE>& result) // std::pair
  {
    decode_tuple(a, result);
  }

  template <typename... ARGS>
  void
  decode(std::any const& a, std::tuple<ARGS...>& result) // std::tuple
  {
    decode_tuple(a, result);
  }

  template <unsigned SIZE, typename TUPLE> // tuple support
  struct per_entry {
    static void decode_tuple_entry(ps_sequence_t const&, TUPLE&);
  };

  template <typename TUPLE> // tuple support
  struct per_entry<0, TUPLE> {
    static void decode_tuple_entry(ps_sequence_t const&, TUPLE&);
  };

  template <class T>
    requires(!std::is_arithmetic_v<T>)
  void decode(std::any const&, T&); // none of the above

} // fhicl::detail

// ======================================================================

template <std::unsigned_integral T> // unsigned
// requires std::unsigned_integral<T>
fhicl::detail::ps_atom_t
fhicl::detail::encode(T const& value)
{
  return encode(uintmax_t(value));
}

template <std::signed_integral T> // signed
// requires std::signed_integral<T>
fhicl::detail::ps_atom_t
fhicl::detail::encode(T const& value)
{
  return encode(intmax_t(value));
}

template <std::floating_point T> // floating-point
// requires std::floating_point<T>
fhicl::detail::ps_atom_t
fhicl::detail::encode(T const& value)
{
  return encode(ldbl(value));
}

template <class T> // complex
fhicl::detail::ps_atom_t
fhicl::detail::encode(std::complex<T> const& value)
{
  return '(' + encode(value.real()) + ',' + encode(value.imag()) + ')';
}

template <class T> // sequence
fhicl::detail::ps_sequence_t
fhicl::detail::encode(std::vector<T> const& value)
{
  ps_sequence_t result;
  for (auto const& e : value) {
    result.emplace_back(encode(e));
  }
  return result;
}

template <fhicl::detail::non_numeric T> // none of the above
std::string
fhicl::detail::encode(T const& value)
{
  return boost::lexical_cast<std::string>(value);
}

// ----------------------------------------------------------------------

//===================================================================
// unsigned
template <std::unsigned_integral T>
void
fhicl::detail::decode(std::any const& a, T& result)
{
  std::uintmax_t via;
  decode(a, via);
  result = boost::numeric_cast<T>(via);
}

//====================================================================
// signed
template <std::signed_integral T>
void
fhicl::detail::decode(std::any const& a, T& result)
{
  std::intmax_t via;
  decode(a, via);
  result = boost::numeric_cast<T>(via);
}

//====================================================================
// floating-point
template <std::floating_point T>
void
fhicl::detail::decode(std::any const& a, T& result)
{
  ldbl via;
  decode(a, via);
  result = via; // boost::numeric_cast<T>(via);
}

//====================================================================
// complex
template <class T>
void
fhicl::detail::decode(std::any const& a, std::complex<T>& result)
{
  std::complex<ldbl> via;
  decode(a, via);
  result = {boost::numeric_cast<T>(via.real()),
            boost::numeric_cast<T>(via.imag())};
}

//====================================================================
// sequence
template <class T>
void
fhicl::detail::decode(std::any const& a, std::vector<T>& result)
{
  if (a.type() == typeid(std::string)) {
    std::string str;
    decode(a, str);

    extended_value xval;
    std::string unparsed;
    if (!parse_value_string(str, xval, unparsed) || !xval.is_a(SEQUENCE))
      throw fhicl::exception(type_mismatch, "error in sequence string:\n")
        << str << "\nat or before:\n"
        << unparsed;

    auto const& seq = fhicl::extended_value::sequence_t(xval);
    result.clear();
    T via;
    for (auto const& e : seq) {
      decode(e.to_string(), via);
      result.push_back(via);
    }
  }

  else if (a.type() == typeid(ps_sequence_t)) {
    ps_sequence_t const& seq = std::any_cast<ps_sequence_t>(a);
    result.clear();
    T via;
    for (auto const& e : seq) {
      decode(e, via);
      result.push_back(via);
    }
  }

  else
    throw fhicl::exception(type_mismatch, "invalid sequence");
}

//====================================================================
// per-entry decode

namespace fhicl::detail {
  template <typename TUPLE, size_t I>
  auto
  decode_entry(ps_sequence_t const& vec)
  {
    std::tuple_element_t<I, TUPLE> result_elem;
    decode(vec.at(I), result_elem);
    return result_elem;
  }

  template <typename TUPLE, size_t... I>
  void
  decode_tuple_entries(ps_sequence_t const& vec,
                       TUPLE& result,
                       std::index_sequence<I...>)
  {
    ((std::get<I>(result) = decode_entry<TUPLE, I>(vec)), ...);
  }
}

// tuple-type support
template <typename U>
void
fhicl::detail::decode_tuple(std::any const& a, U& result)
{
  auto const seq = std::any_cast<ps_sequence_t>(a);

  constexpr std::size_t TUPLE_SIZE = std::tuple_size_v<U>;

  if (seq.size() != TUPLE_SIZE) {
    std::ostringstream errmsg;
    errmsg << "Number of expected arguments (" << TUPLE_SIZE
           << ") does not match "
           << "number of FHiCL sequence entries (" << seq.size() << "): [ ";
    for (auto ca = seq.begin(); ca != seq.cend(); ++ca) {
      std::string tmp;
      decode(*ca, tmp);
      errmsg << tmp;
      if (ca != seq.cend() - 1) {
        errmsg << ", ";
      }
    }
    errmsg << " ]";
    throw std::length_error(errmsg.str());
  }

  decode_tuple_entries(seq, result, std::make_index_sequence<TUPLE_SIZE>());
  //  per_entry<TUPLE_SIZE - 1, U>::decode_tuple_entry(seq, result);
}

//====================================================================
template <class T> // none of the above
  requires(!std::is_arithmetic_v<T>)
void
fhicl::detail::decode(std::any const& a, T& result)
{
  result = std::any_cast<T>(a);
}

// ======================================================================

#endif /* fhiclcpp_coding_h */

// Local Variables:
// mode: c++
// End:
