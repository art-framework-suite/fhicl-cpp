#include "fhiclcpp/types/detail/TableBase.h"
#include "fhiclcpp/types/detail/TableMemberRegistry.h"

namespace fhicl::detail {

  TableBase::TableBase(Name const& name,
                       Comment const& comment,
                       par_style const vt,
                       std::function<bool()> maybeUse)
    : ParameterBase{name, comment, vt, par_type::TABLE, move(maybeUse)}
  {}

  std::vector<cet::exempt_ptr<ParameterBase>> const&
  TableBase::members() const
  {
    return members_;
  }

  void
  TableBase::finalize_members()
  {
    // This must be called in a derived class **after** the table's
    // values has been set.
    members_ = TableMemberRegistry::release_members();
  }

  void
  TableBase::maybe_implicitly_default()
  {
    bool const implicitly_default =
      std::all_of(members_.begin(), members_.end(), [](auto p) {
        return p->has_default() || p->is_optional();
      });
    if (implicitly_default)
      set_par_style(par_style::DEFAULT);
  }

} // namespace fhicl::detail
