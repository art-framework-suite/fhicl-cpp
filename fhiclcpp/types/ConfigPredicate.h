#ifndef fhiclcpp_types_ConfigPredicate_h
#define fhiclcpp_types_ConfigPredicate_h

#include <functional>

namespace fhicl {

  namespace detail {
    inline auto AlwaysUse = []() { return true; };
  }

  template <typename T>
  using NullaryConfigPredicate_t = bool (T::*)() const;

  template <typename T>
  auto
  use_if(T* p, NullaryConfigPredicate_t<T> f)
  {
    return [p, f]() { return (p->*f)(); };
  }

  template <typename T>
  auto
  use_unless(T* p, NullaryConfigPredicate_t<T> f)
  {
    return [p, f]() { return !(p->*f)(); };
  }
}

#endif /* fhiclcpp_types_ConfigPredicate_h */

// Local variables:
// mode: c++
// End:
