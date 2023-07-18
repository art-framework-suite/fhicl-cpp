#include "fhiclcpp/types/DelegatedParameter.h"
#include "fhiclcpp/types/ConfigPredicate.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"

namespace fhicl {

  DelegatedParameter::DelegatedParameter(Name&& name)
    : DelegatedParameter{std::move(name), Comment("")}
  {}

  DelegatedParameter::DelegatedParameter(Name&& name, Comment&& comment)
    : DelegateBase{std::move(name),
                   std::move(comment),
                   par_style::REQUIRED,
                   detail::AlwaysUse()}
    , RegisterIfTableMember{this}
  {
    NameStackRegistry::end_of_ctor();
  }

  template <fhicl::maybe_use_param F>
  DelegatedParameter::DelegatedParameter(Name&& name,
                                         Comment&& comment,
                                         F maybeUse)
    : DelegateBase{std::move(name),
                   std::move(comment),
                   par_style::REQUIRED_CONDITIONAL,
                   maybeUse}
    , RegisterIfTableMember{this}
  {
    NameStackRegistry::end_of_ctor();
  }
}
