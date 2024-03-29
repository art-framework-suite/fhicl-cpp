#ifndef fhiclcpp_types_OptionalTable_h
#define fhiclcpp_types_OptionalTable_h

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/type_traits.h"
#include "fhiclcpp/types/Comment.h"
#include "fhiclcpp/types/ConfigPredicate.h"
#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterArgumentTypes.h"
#include "fhiclcpp/types/detail/ParameterMetadata.h"
#include "fhiclcpp/types/detail/TableBase.h"
#include "fhiclcpp/types/detail/TableMemberRegistry.h"
#include "fhiclcpp/types/detail/type_traits_error_msgs.h"

#include <memory>
#include <set>
#include <string>

namespace fhicl {

  //========================================================
  template <table_or_atom_compatible T>
  class OptionalTable final : public detail::TableBase,
                              private detail::RegisterIfTableMember {
  public:
    //=====================================================
    // User-friendly
    // ... c'tors
    explicit OptionalTable(Name&& name);
    explicit OptionalTable(Name&& name, Comment&& comment);
    template <fhicl::maybe_use_param F>
    explicit OptionalTable(Name&& name, Comment&& comment, F maybeUse);
    OptionalTable(ParameterSet const& pset,
                  std::set<std::string> const& keysToIgnore);

    // ... Accessors
    std::optional<T>
    operator()() const
    {
      return hasValue() ? std::make_optional(*value_) : std::nullopt;
    }

    // Obsolete
    bool
    operator()(T& value) const
    {
      auto opt = operator()();
      if (opt) {
        value = *value_;
      }
      return opt.has_value();
    }

    bool
    hasValue() const
    {
      return TableBase::has_value();
    }

    ParameterSet const&
    get_PSet() const
    {
      return TableBase::guarantee_pset();
    }

    //=====================================================
    // Expert-only
    struct fhicl_optional_tag {};
    using value_type = T;

    OptionalTable();

  private:
    std::shared_ptr<T> value_{std::make_shared<T>()};
  };

  //=====================================================
  // Implementation

  template <table_or_atom_compatible T>
  OptionalTable<T>::OptionalTable(Name&& name)
    : OptionalTable{std::move(name), Comment("")}
  {}

  template <table_or_atom_compatible T>
  OptionalTable<T>::OptionalTable(Name&& name, Comment&& comment)
    : TableBase{std::move(name),
                std::move(comment),
                par_style::OPTIONAL,
                detail::AlwaysUse}
    , RegisterIfTableMember{this}
  {
    finalize_members();
    NameStackRegistry::end_of_ctor();
  }

  template <table_or_atom_compatible T>
  template <fhicl::maybe_use_param F>
  OptionalTable<T>::OptionalTable(Name&& name, Comment&& comment, F maybeUse)
    : TableBase{std::move(name),
                std::move(comment),
                par_style::OPTIONAL_CONDITIONAL,
                maybeUse}
    , RegisterIfTableMember{this}
  {
    finalize_members();
    NameStackRegistry::end_of_ctor();
  }

  template <table_or_atom_compatible T>
  OptionalTable<T>::OptionalTable(ParameterSet const& pset,
                                  std::set<std::string> const& keysToIgnore)
    : TableBase{Name("<top_level>"),
                Comment(""),
                par_style::OPTIONAL,
                detail::AlwaysUse}
    , RegisterIfTableMember{this}
  {
    finalize_members();
    NameStackRegistry::end_of_ctor();
    validate(pset, keysToIgnore);
  }

}

#endif /* fhiclcpp_types_OptionalTable_h */

// Local variables:
// mode: c++
// End:
