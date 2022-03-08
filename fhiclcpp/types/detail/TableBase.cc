#include "fhiclcpp/types/detail/TableBase.h"
#include "fhiclcpp/types/detail/PrintAllowedConfiguration.h"
#include "fhiclcpp/types/detail/TableMemberRegistry.h"
#include "fhiclcpp/types/detail/ValidateThenSet.h"
#include "fhiclcpp/types/detail/optional_parameter_message.h"
#include "fhiclcpp/types/detail/validationException.h"

#include <ostream>

namespace fhicl::detail {

  TableBase::TableBase(Name const& name,
                       Comment const& comment,
                       par_style const vt,
                       std::function<bool()> maybeUse)
    : ParameterBase{name, comment, vt, par_type::TABLE, move(maybeUse)}
  {}

  TableBase::~TableBase() = default;

  void
  TableBase::print_allowed_configuration(std::ostream& os,
                                         std::string const& tab) const
  {
    os << '\n' << tab << optional_parameter_message() << '\n';
    PrintAllowedConfiguration pc{os, false, tab};
    pc.walk_over(*this);
  }

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

  void
  TableBase::validate(ParameterSet const& pset,
                      std::set<std::string> const& ignorable_keys)
  {
    pset_ = std::make_optional(pset);
    ValidateThenSet vs{pset, ignorable_keys};
    cet::for_all(members_, [&vs](auto m) { vs.walk_over(*m); });

    try {
      vs.check_keys();
    }
    catch (detail::validationException const&) {
      NameStackRegistry::clear();
      throw;
    }
  }

  bool
  TableBase::has_value() const
  {
    return pset_.has_value();
  }

  ParameterSet const&
  TableBase::get_pset() const
  {
    return pset_.value();
  }

  ParameterSet const&
  TableBase::guarantee_pset() const
  {
    static ParameterSet const empty_pset;
    return pset_.has_value() ? *pset_ : empty_pset;
  }

  std::optional<ParameterSet>
  TableBase::select_pset(ParameterSet const& pset) const
  {
    // Kind of tricky: we do not have the name of the current
    // parameter set.  A placeholder is often used (e.g. "<top_level>").
    // Fortunately, since the pset is passed in, we can just assign to
    // it for a top-level ParameterSet.  However, for nested parameter
    // sets, we need to trim off the placeholder, and then the key we
    // send pset.get<fhicl::ParameterSet>(key) is the key relative to
    // the top-level pset.
    std::string const& rkey = key();
    std::string const& nkey = strip_first_containing_name(rkey);
    if (nkey == rkey) {
      return std::make_optional(pset);
    }
    return pset.get<ParameterSet>(nkey);
  }

  std::optional<ParameterSet>
  TableBase::maybe_select_pset(fhicl::ParameterSet const& pset) const
  {
    // Kind of tricky: we do not have the name of the current
    // parameter set.  A placeholder is often used
    // (e.g. "<top_level>").  Fortunately, since the pset is passed
    // in, we can just assign to it for a top-level ParameterSet.
    // However, for nested parameter sets, we need to trim off the
    // placeholder, and then the key we send
    // pset.get<fhicl::ParameterSet>(key) is the key relative to the
    // top-level pset.
    std::string const& rkey = key();
    std::string const& nkey = strip_first_containing_name(rkey);
    if (nkey == rkey) {
      return std::make_optional(pset);
    }
    return pset.get_if_present<ParameterSet>(nkey);
  }

  void
  TableBase::do_set_value(fhicl::ParameterSet const& pset)
  {
    if (is_optional()) {
      pset_ = maybe_select_pset(pset);
    } else {
      pset_ = select_pset(pset);
    }
  }

  std::ostream&
  operator<<(std::ostream& os, TableBase const& t)
  {
    std::ostringstream config;
    t.print_allowed_configuration(config);
    return os << config.str();
  }
} // namespace fhicl::detail
