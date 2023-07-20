#ifndef fhiclcpp_types_detail_maybe_insert_h
#define fhiclcpp_types_detail_maybe_insert_h

#include <ostream>
#include <sstream>
#include <string>
#include <utility>

namespace fhicl::detail {

  template <typename T>
  concept has_insertion_operator = requires(T t, std::ostream os){
    os << t;
  };

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
