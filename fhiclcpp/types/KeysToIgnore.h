#ifndef fhiclcpp_types_KeysToIgnore_h
#define fhiclcpp_types_KeysToIgnore_h

#include "fhiclcpp/type_traits.h"

#include <set>
#include <string>
#include <type_traits>

namespace fhicl {
  namespace detail {

    inline std::set<std::string>&
    concatenate_keys(std::set<std::string>& keys)
    {
      return keys;
    }

    template <typename H, typename... T>
    std::set<std::string>&
    concatenate_keys(std::set<std::string>& keys, H const& h, T const&... t)
    {
      keys.insert(begin(h), end(h));
      return concatenate_keys(keys, t...);
    }

  } // detail

  template <typename H, typename... T>
  requires std::invocable<H> && detail::invocable_pack<T...>
  struct KeysToIgnore {
    std::set<std::string>
    operator()()
    {
      std::set<std::string> keys_to_ignore{H{}()};
      return detail::concatenate_keys(keys_to_ignore,
                                      T{}()...);
    }
  };
}

#endif /* fhiclcpp_types_KeysToIgnore_h */

// Local variables:
// mode: c++
// End:
