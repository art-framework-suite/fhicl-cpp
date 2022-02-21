#ifndef fhiclcpp_types_detail_validationException_h
#define fhiclcpp_types_detail_validationException_h

#include <exception>
#include <string>

namespace fhicl::detail {
  class validationException : public std::exception {
  public:
    validationException(std::string msg) : msg_{std::move(msg)} {}
    char const*
    what() const noexcept override
    {
      return msg_.c_str();
    }

  private:
    std::string msg_;
  };
}

#endif /* fhiclcpp_types_detail_validationException_h */

// Local variables:
// mode:c++
// End:
