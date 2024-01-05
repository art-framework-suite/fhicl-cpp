#include "fhiclcpp/types/detail/ParameterBase.h"

namespace fhicl::detail {

  std::string const&
  ParameterBase::key() const
  {
    return mdata_.key();
  }
  std::string const&
  ParameterBase::name() const
  {
    return mdata_.name();
  }
  std::string const&
  ParameterBase::comment() const
  {
    return mdata_.comment();
  }
  bool
  ParameterBase::has_default() const
  {
    return mdata_.has_default();
  }
  bool
  ParameterBase::is_optional() const
  {
    return mdata_.is_optional();
  }
  bool
  ParameterBase::is_conditional() const
  {
    return mdata_.is_conditional();
  }
  par_type
  ParameterBase::parameter_type() const
  {
    return mdata_.type();
  }
  bool
  ParameterBase::should_use() const
  {
    return maybeUse_();
  }

  ParameterBase::~ParameterBase() = default;

  // Modifiers
  void
  ParameterBase::set_value(fhicl::ParameterSet const& ps)
  {
    do_set_value(ps);
  }
  bool
  ParameterBase::preset_value(fhicl::ParameterSet const& ps)
  {
    return do_preset_value(ps);
  }
  void
  ParameterBase::set_par_style(par_style const vt)
  {
    mdata_.set_par_style(vt);
  }

  bool
  ParameterBase::do_preset_value(fhicl::ParameterSet const&)
  {
    return false;
  }

} // namespace fhicl::detail
