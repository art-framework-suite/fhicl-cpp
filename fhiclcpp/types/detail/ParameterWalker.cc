#include "fhiclcpp/types/detail/ParameterWalker.h"

namespace fhicl::detail {
  // Explicit instantiations to avoid code bloat
  template class ParameterWalker<tt::const_flavor::require_const>;
  template class ParameterWalker<tt::const_flavor::require_non_const>;
}
