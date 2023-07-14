#ifndef fhiclcpp_type_traits_h
#define fhiclcpp_type_traits_h
// ======================================================================
//
// type traits
//
// ======================================================================

#include "cetlib/metaprogramming.h"

#include <array>
#include <complex>
#include <functional>
#include <set>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

// ======================================================================

namespace fhicl {
  namespace detail {
    template <typename T>
    concept numeric = std::is_arithmetic_v<T>;

    template <typename T>
    concept non_numeric = !
    numeric<T>;
    
  }

  template <typename T>
  concept keys_to_ignore_provider =
    requires(T t) {
      {
        std::invoke(std::forward<T>(t))
        } -> std::same_as<std::set<std::string>>;
    };
 

  template <typename T>
  class Atom;
  template <typename T>
  class OptionalAtom;

  template <typename T, keys_to_ignore_provider... KeysToIgnore>
  class Table;
  template <typename T>
  class OptionalTable;

  template <typename T>
  class TableFragment;

  template <typename T>
  concept is_table_fragment = std::same_as<T, typename ::fhicl::TableFragment<T>>;

  template <typename... ARGS>
  class Tuple;
  template <typename... ARGS>
  class OptionalTuple;

  template <typename T, std::size_t SZ>
  class Sequence;
  template <typename T, std::size_t SZ>
  class OptionalSequence;

  template <typename T, typename... ARGS>
  class TupleAs;
  template <typename T, typename... ARGS>
  class OptionalTupleAs;

  template <typename T, typename Config>
  class TableAs;
  template <typename T, typename Config>
  class OptionalTableAs;

  class DelegatedParameter;
  class OptionalDelegatedParameter;
}

namespace tt {

  using cet::enable_if_function_exists_t;

  template <bool b, typename T = void>
  using disable_if = std::enable_if<!b, T>;

  template <bool b, typename T = void>
  using disable_if_t = typename disable_if<b, T>::type;

  template <typename T>
  struct is_int
    : std::bool_constant<std::is_integral_v<T> && !std::is_unsigned_v<T>> {};

  template <typename T>
  using is_numeric = std::is_arithmetic<T>;

  template <typename T>
  using is_uint = std::is_unsigned<T>;

  template <typename T, typename = void>
  struct is_callable : std::false_type {};

  template <typename T>
  struct is_callable<
    T,
    enable_if_function_exists_t<std::set<std::string> (T::*)(), &T::operator()>>
    : std::true_type {};

  template <typename T>
  struct is_callable<
    T,
    enable_if_function_exists_t<std::set<std::string> (T::*)() const,
                                &T::operator()>> : std::true_type {};

  //=======================================================
  // Enforce (non)const-ness
  //
  enum class const_flavor { require_non_const, require_const };

  template <typename T, const_flavor C>
  struct maybe_const {
    using type = T;
  };

  template <typename T>
  struct maybe_const<T, const_flavor::require_const> {
    using type = std::add_const_t<T>;
  };

  template <typename T, const_flavor C>
  using maybe_const_t = typename maybe_const<T, C>::type;

  //=======================================================
  // Check if sequence type
  //
  template <typename Container>
  struct is_sequence_type : std::false_type {};

  template <typename ARG, std::size_t SZ>
  struct is_sequence_type<std::array<ARG, SZ>> : std::true_type {};
  template <typename... ARGS>
  struct is_sequence_type<std::tuple<ARGS...>> : std::true_type {};
  template <typename... ARGS>
  struct is_sequence_type<std::vector<ARGS...>> : std::true_type {};

  template <typename T>
  inline constexpr bool is_sequence_type_v{is_sequence_type<T>::value};

  //=======================================================
  // Check if Table<>
  //
  template <typename T>
  struct is_table : std::false_type {};

  template <typename T, fhicl::keys_to_ignore_provider... KeysToIgnore>
  struct is_table<fhicl::Table<T, KeysToIgnore...>> : std::true_type {};

  template <typename T>
  inline constexpr bool is_table_v{is_table<T>::value};

  //=======================================================
  // Check if TableFragment<>
  //
  template <typename T>
  struct is_table_fragment : std::false_type {};

  template <typename T>
  struct is_table_fragment<fhicl::TableFragment<T>> : std::true_type {};

  template <typename T>
  inline constexpr bool is_table_fragment_v{is_table_fragment<T>::value};

  //=======================================================
  // Check if optional parameter
  //
  template <typename T>
  struct is_optional_parameter : std::false_type {};

  template <typename T>
  struct is_optional_parameter<fhicl::OptionalTable<T>> : std::true_type {};

  template <typename T>
  struct is_optional_parameter<fhicl::OptionalAtom<T>> : std::true_type {};

  template <typename T, std::size_t SZ>
  struct is_optional_parameter<fhicl::OptionalSequence<T, SZ>>
    : std::true_type {};

  template <typename... TYPES>
  struct is_optional_parameter<fhicl::OptionalTuple<TYPES...>>
    : std::true_type {};

  template <typename T, typename... ARGS>
  struct is_optional_parameter<fhicl::OptionalTupleAs<T(ARGS...)>>
    : std::true_type {};

  template <typename T>
  inline constexpr bool is_optional_parameter_v{
    is_optional_parameter<T>::value};

  //=======================================================
  // Check if delegated parameter
  //
  template <typename T>
  struct is_delegated_parameter : std::false_type {};

  template <>
  struct is_delegated_parameter<fhicl::DelegatedParameter> : std::true_type {};

  template <>
  struct is_delegated_parameter<fhicl::OptionalDelegatedParameter>
    : std::true_type {};

  template <typename T>
  inline constexpr bool is_delegated_parameter_v{
    is_delegated_parameter<T>::value};

  //=======================================================
  // Check if fhicl type -- i.e. Atom<>, Table<>, etc.
  //
  template <typename T>
  struct is_fhicl_type : std::false_type {};

  // ... Table
  template <typename T, fhicl::keys_to_ignore_provider... KeysToIgnore>
  struct is_fhicl_type<fhicl::Table<T, KeysToIgnore...>> : std::true_type {};

  template <typename T>
  struct is_fhicl_type<fhicl::OptionalTable<T>> : std::true_type {};

  // ... Atom
  template <typename T>
  struct is_fhicl_type<fhicl::Atom<T>> : std::true_type {};

  template <typename T>
  struct is_fhicl_type<fhicl::OptionalAtom<T>> : std::true_type {};

  // ... Sequence
  template <typename T, std::size_t SZ>
  struct is_fhicl_type<fhicl::Sequence<T, SZ>> : std::true_type {};

  template <typename T, std::size_t SZ>
  struct is_fhicl_type<fhicl::OptionalSequence<T, SZ>> : std::true_type {};

  // ... Tuple
  template <typename... TYPES>
  struct is_fhicl_type<fhicl::Tuple<TYPES...>> : std::true_type {};

  template <typename... TYPES>
  struct is_fhicl_type<fhicl::OptionalTuple<TYPES...>> : std::true_type {};

  // ... TupleAs
  template <typename T, typename... ARGS>
  struct is_fhicl_type<fhicl::TupleAs<T(ARGS...)>> : std::true_type {};

  template <typename T, typename... ARGS>
  struct is_fhicl_type<fhicl::OptionalTupleAs<T(ARGS...)>> : std::true_type {};

  // ... TableAs
  template <typename T, typename Config>
  struct is_fhicl_type<fhicl::TableAs<T, Config>> : std::true_type {};

  template <typename T, typename Config>
  struct is_fhicl_type<fhicl::OptionalTableAs<T, Config>> : std::true_type {};

  template <typename T>
  inline constexpr bool is_fhicl_type_v{is_fhicl_type<T>::value};

  //=======================================================
  // Get FHiCL types: can be Atom, Sequence, Tuple, or Table
  //
  template <typename T>
  struct fhicl_type_impl {
    using type = fhicl::Atom<T>;
  };

  template <typename T, std::size_t SZ>
  struct fhicl_type_impl<fhicl::Sequence<T, SZ>> {
    using type = fhicl::Sequence<T, SZ>;
  };

  template <typename T, fhicl::keys_to_ignore_provider... KeysToIgnore>
  struct fhicl_type_impl<fhicl::Table<T, KeysToIgnore...>> {
    using type = fhicl::Table<T, KeysToIgnore...>;
  };

  template <typename... ARGS>
  struct fhicl_type_impl<fhicl::Tuple<ARGS...>> {
    using type = fhicl::Tuple<ARGS...>;
  };

  template <typename T, typename Config>
  struct fhicl_type_impl<fhicl::TableAs<T, Config>> {
    using type = fhicl::TableAs<T, Config>;
  };

  template <typename T, typename... ARGS>
  struct fhicl_type_impl<fhicl::TupleAs<T(ARGS...)>> {
    using type = fhicl::TupleAs<T(ARGS...)>;
  };

  template <typename T>
  struct fhicl_type_impl<fhicl::OptionalAtom<T>> {
    using type = fhicl::OptionalAtom<T>;
  };

  template <typename T, std::size_t SZ>
  struct fhicl_type_impl<fhicl::OptionalSequence<T, SZ>> {
    using type = fhicl::OptionalSequence<T, SZ>;
  };

  template <typename T>
  struct fhicl_type_impl<fhicl::OptionalTable<T>> {
    using type = fhicl::OptionalTable<T>;
  };

  template <typename... ARGS>
  struct fhicl_type_impl<fhicl::OptionalTuple<ARGS...>> {
    using type = fhicl::OptionalTuple<ARGS...>;
  };

  template <typename T, typename Config>
  struct fhicl_type_impl<fhicl::OptionalTableAs<T, Config>> {
    using type = fhicl::OptionalTableAs<T, Config>;
  };

  template <typename T, typename... ARGS>
  struct fhicl_type_impl<fhicl::OptionalTupleAs<T(ARGS...)>> {
    using type = fhicl::OptionalTupleAs<T(ARGS...)>;
  };

  // The alias
  template <typename T>
  using fhicl_type = typename fhicl_type_impl<T>::type;

  //=======================================================
  // Get Return types
  //
  template <typename T>
  struct return_type_impl {
    using value_type = T;
  };

  template <typename T>
  struct return_type_impl<fhicl::Atom<T>> {
    using value_type = typename fhicl::Atom<T>::value_type;
  };

  template <typename T, std::size_t SZ>
  struct return_type_impl<fhicl::Sequence<T, SZ>> {
    using value_type = typename fhicl::Sequence<T, SZ>::value_type;
  };

  template <typename S, fhicl::keys_to_ignore_provider... KeysToIgnore>
  struct return_type_impl<fhicl::Table<S, KeysToIgnore...>> {
    using value_type = typename fhicl::Table<S, KeysToIgnore...>::value_type;
  };

  template <typename... ARGS>
  struct return_type_impl<fhicl::Tuple<ARGS...>> {
    using value_type = typename fhicl::Tuple<ARGS...>::value_type;
  };

  template <typename T, typename Config>
  struct return_type_impl<fhicl::TableAs<T, Config>> {
    using value_type = typename fhicl::TableAs<T, Config>::value_type;
  };

  template <typename T, typename... ARGS>
  struct return_type_impl<fhicl::TupleAs<T(ARGS...)>> {
    using value_type = typename fhicl::TupleAs<T(ARGS...)>::value_type;
  };

  // The alias
  template <typename... ARGS>
  using return_type = typename return_type_impl<ARGS...>::value_type;
}

#endif /* fhiclcpp_type_traits_h */

// Local Variables:
// mode: c++
// End:
