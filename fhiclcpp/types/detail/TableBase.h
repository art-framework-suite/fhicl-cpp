#ifndef fhiclcpp_types_detail_TableBase_h
#define fhiclcpp_types_detail_TableBase_h

#include "cetlib/exempt_ptr.h"
#include "fhiclcpp/fwd.h"
#include "fhiclcpp/types/detail/ParameterBase.h"

namespace fhicl::detail {
  class TableBase : public ParameterBase {
  public:
    TableBase(Name const& name,
              Comment const& comment,
              par_style vt,
              std::function<bool()> maybeUse);

    std::vector<cet::exempt_ptr<ParameterBase>> const& members() const;

  protected:
    void finalize_members();
    void maybe_implicitly_default();

  private:
    std::vector<cet::exempt_ptr<ParameterBase>> members_;
  };
}

#endif /* fhiclcpp_types_detail_TableBase_h */

// Local variables:
// mode: c++
// End:
