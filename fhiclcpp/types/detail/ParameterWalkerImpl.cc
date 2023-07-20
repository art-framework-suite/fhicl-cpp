#include "fhiclcpp/types/detail/ParameterWalkerImpl.h"

namespace fhicl::detail {
  // Explicit instantiations to avoid code bloat
  template class ParameterWalkerImpl<tt::const_flavor::require_const>;
  template class ParameterWalkerImpl<tt::const_flavor::require_non_const>;
}
