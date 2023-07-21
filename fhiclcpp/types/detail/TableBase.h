#ifndef fhiclcpp_types_detail_TableBase_h
#define fhiclcpp_types_detail_TableBase_h

#include "cetlib/exempt_ptr.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/fwd.h"
#include "fhiclcpp/types/detail/ParameterBase.h"

#include <iosfwd>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace fhicl::detail {
  class TableBase : public ParameterBase {
  public:
    template <fhicl::maybe_use_param F>
    TableBase(Name const& name,
              Comment const& comment,
              par_style vt,
              F maybeUse);
    ~TableBase();

    void print_allowed_configuration(
      std::ostream& os,
      std::string const& tab = std::string(3, ' ')) const;
    std::vector<cet::exempt_ptr<ParameterBase>> const& members() const;
    void validate(ParameterSet const& pset,
                  std::set<std::string> const& ignorable_keys = {});

    struct fhicl_table_tag {};

  protected:
    void finalize_members();
    void maybe_implicitly_default();
    bool has_value() const;
    ParameterSet const& get_pset() const;
    ParameterSet const& guarantee_pset() const;

  private:
    void do_set_value(ParameterSet const& pset) final;
    std::optional<ParameterSet> select_pset(ParameterSet const& pset) const;
    std::optional<ParameterSet> maybe_select_pset(
      ParameterSet const& pset) const;

    std::optional<ParameterSet> pset_;
    std::vector<cet::exempt_ptr<ParameterBase>> members_;
  };

  // Insertion operator
  std::ostream& operator<<(std::ostream& os, TableBase const& t);
}

template <fhicl::maybe_use_param F>
fhicl::detail::TableBase::TableBase(Name const& name,
                                    Comment const& comment,
                                    par_style const vt,
                                    F maybeUse)
  : ParameterBase{name, comment, vt, par_type::TABLE, std::move(maybeUse)}
{}

#endif /* fhiclcpp_types_detail_TableBase_h */

// Local variables:
// mode: c++
// End:
