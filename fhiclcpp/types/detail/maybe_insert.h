#ifndef fhiclcpp_types_detail_maybe_insert_h
#define fhiclcpp_types_detail_maybe_insert_h

#include <ostream>
#include <sstream>
#include <string>
#include <utility>

namespace fhicl::detail {
  template <typename T, typename = void>
  struct maybe_insert {
    static std::string
    emit(T const&)
    {
      return "     A default value is present, but it cannot be\n"
             "     printed out since no 'operator<<' overload has\n"
             "     been provided for the above type.";
    }
  };

  template <typename T>
  using insertion_expression_t =
    decltype(std::declval<std::ostream>() << std::declval<T>());

  template <typename T>
  struct maybe_insert<T, std::void_t<insertion_expression_t<T>>> {
    static std::string
    emit(T const& t)
    {
      std::ostringstream os;
      os << "     with a default value of:\n"
         << "        " << t;
      return os.str();
    }
  };
}

#endif /* fhiclcpp_types_detail_maybe_insert_h */

// Local variables:
// mode: c++
// End:
