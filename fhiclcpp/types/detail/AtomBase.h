#ifndef fhiclcpp_types_detail_AtomBase_h
#define fhiclcpp_types_detail_AtomBase_h

#include "fhiclcpp/type_traits.h"
#include "fhiclcpp/types/ConfigPredicate.h"
#include "fhiclcpp/types/detail/ParameterBase.h"

namespace fhicl::detail {
  class AtomBase : public ParameterBase {
  public:
    template <fhicl::maybe_use_param F>
    AtomBase(Name const& name,
             Comment const& comment,
             par_style const vt,
             F maybeUse)
      : ParameterBase{name, comment, vt, par_type::ATOM, maybeUse}
    {}

    std::string
    stringified_value() const
    {
      return get_stringified_value();
    }

  private:
    virtual std::string get_stringified_value() const = 0;
  };
}

#endif /* fhiclcpp_types_detail_AtomBase_h */

// Local variables:
// mode: c++
// End:
