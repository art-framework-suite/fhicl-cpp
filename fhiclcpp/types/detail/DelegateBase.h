#ifndef fhiclcpp_types_detail_DelegateBase_h
#define fhiclcpp_types_detail_DelegateBase_h

#include "fhiclcpp/types/ConfigPredicate.h"
#include "fhiclcpp/types/detail/ParameterBase.h"

namespace fhicl::detail {
  class DelegateBase : public ParameterBase {
  public:
    template <fhicl::maybe_use_param F>
    DelegateBase(Name const& name,
                 Comment const& comment,
                 par_style const vt,
                 F maybeUse)
      : ParameterBase{name, comment, vt, par_type::DELEGATE, maybeUse}
    {}
  };
}

#endif /* fhiclcpp_types_detail_DelegateBase_h */

// Local variables:
// mode: c++
// End:
