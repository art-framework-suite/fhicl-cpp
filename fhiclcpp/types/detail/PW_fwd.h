#ifndef fhiclcpp_types_detail_PW_fwd_h
#define fhiclcpp_types_detail_PW_fwd_h

namespace tt {
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
}

namespace fhicl::detail {
  template <tt::const_flavor C> class ParameterWalkerImpl;
}

#endif /* fhiclcpp_types_detail_PW_fwd_h */

// Local variables:
// mode: c++
// End:


