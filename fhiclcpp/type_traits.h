#ifndef fhiclcpp_type_traits_h
#define fhiclcpp_type_traits_h
// ======================================================================
//
// type traits
//
// ======================================================================

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

  template <typename T>
  concept is_table_fragment_param =
    std::same_as<T, typename ::fhicl::TableFragment<T>>;

  template <typename T>
  concept is_optional_param = std::is_base_of_v<::fhicl::OptionalTable, T> ||
                              std::is_base_of_v<::fhicl::OptionalAtom, T> ||
                              std::is_base_of_v<::fhicl::OptionalSequence, T> ||
                              std::is_base_of_v<::fhicl::OptionalTuple, T> ||
                              std::is_base_of_v<::fhicl::OptionalTupleAs, T>;

  template <typename T>
  concept is_delegated_param =
    std::is_base_of_v<::fhicl::DelegatedParameter, T> ||
    std::is_base_of_v<::fhicl::OptionalDelegatedParameter, T>;

  template <typename T>
  concept is_table_param = std::is_base_of_v<::fhicl::Table, T>;

  template <typename T>
  concept is_sequence_type_param =
    std::is_base_of_v<std::array, T> || std::is_base_of_v<std::tuple, T> ||
    std::is_base_of_v<std::vector, T>;

  template <typename T>
  concept is_fhicl_type_param =
    is_optional_param<T> || std::is_base_of_v<::fhicl::Atom, T> ||
    std::is_base_of_v<::fhicl::Sequence, T> ||
    std::is_base_of_v<::fhicl::Tuple, T> ||
    std::is_base_of_v<::fhicl::TupleAs, T> ||
    std::is_base_of_v<::fhicl::TableAs, T>;

  template <typename T>
  concept maybe_use_param = std::convertible_to<T, std::function<bool()>>;
}

namespace tt {

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
