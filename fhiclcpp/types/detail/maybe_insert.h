#ifndef fhiclcpp_types_detail_maybe_insert_h
#define fhiclcpp_types_detail_maybe_insert_h

#include <ostream>
#include <sstream>
#include <string>
#include <utility>

namespace fhicl::detail {
  template <typename T>
  using insertion_expression_t =
    decltype(std::declval<std::ostream&>() << std::declval<T const&>());

  template <typename T, typename = void>
  constexpr bool has_insertion_operator{false};

  template <typename T>
  constexpr bool
    has_insertion_operator<T, std::void_t<insertion_expression_t<T>>>{true};

  template <typename T>
  std::string
  maybe_insert(T const& t)
  {
    if constexpr (has_insertion_operator<T>) {
      std::ostringstream os;
      os << "     with a default value of:\n"
         << "        " << t;
      return os.str();
    }
    return "     A default value is present, but it cannot be\n"
           "     printed out since no 'operator<<' overload has\n"
           "     been provided for the above type.";
  }
}

#endif /* fhiclcpp_types_detail_maybe_insert_h */

// Local variables:
// mode: c++
// End:
