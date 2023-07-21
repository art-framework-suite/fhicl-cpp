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
  concept maybe_use_param = std::convertible_to<T, std::function<bool()>>;

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

  //////////////////////////////////////////////////////////////////////
  // FHiCL type identification

  // Sequence types
  template <typename T>
  struct is_sequence_impl : std::false_type {};
  template <typename T, std::size_t SZ>
  struct is_sequence_impl<std::array<T, SZ>> : std::true_type {};
  template <typename... ARGS>
  struct is_sequence_impl<std::tuple<ARGS...>> : std::true_type {};
  template <typename... ARGS>
  struct is_sequence_impl<std::vector<ARGS...>> : std::true_type {};
  
  template <typename T>
  concept is_sequence_type_param = is_sequence_impl<T>::value;

  // FHiCL type attributes
  template <typename T>
  concept is_sequence_param = requires { typename T::fhicl_sequence_tag; };
  template <typename T>
  concept is_table_param = requires { typename T::fhicl_table_tag; };
  template <typename T>
  concept is_table_fragment_param = requires { typename T::fhicl_table_fragment_tag; };
  template <typename T>
  concept is_optional_param = requires { typename T::fhicl_optional_tag; };
  template <typename T>
  concept is_delegated_param = requires { typename T::fhicl_delegate_tag; };
  template <typename T>
  concept is_fhicl_type_param = requires { typename T::fhicl_type_tag; };
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
