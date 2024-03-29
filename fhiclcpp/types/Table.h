#ifndef fhiclcpp_types_Table_h
#define fhiclcpp_types_Table_h

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/type_traits.h"
#include "fhiclcpp/types/Comment.h"
#include "fhiclcpp/types/ConfigPredicate.h"
#include "fhiclcpp/types/KeysToIgnore.h"
#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterArgumentTypes.h"
#include "fhiclcpp/types/detail/ParameterMetadata.h"
#include "fhiclcpp/types/detail/TableBase.h"
#include "fhiclcpp/types/detail/TableMemberRegistry.h"
#include "fhiclcpp/types/detail/ValidateThenSet.h"
#include "fhiclcpp/types/detail/type_traits_error_msgs.h"
#include "fhiclcpp/types/detail/validationException.h"

#include <memory>
#include <ostream>
#include <set>
#include <string>

namespace fhicl {

  //========================================================
  template <table_or_atom_compatible T, keys_to_ignore_provider... KeysToIgnore>
  class Table final : public detail::TableBase,
                      private detail::RegisterIfTableMember {
  public:
    //=====================================================
    // User-friendly
    // ... c'tors
    template <typename... TCARGS>
    explicit Table(Name&& name, TCARGS&&... tcargs);
    template <typename... TCARGS>
    explicit Table(Name&& name, Comment&& comment, TCARGS&&... tcargs);
    template <fhicl::maybe_use_param F, typename... TCARGS>
    explicit Table(Name&& name,
                   Comment&& comment,
                   F maybeUse,
                   TCARGS&&... tcargs);

    Table(ParameterSet const& pset,
          std::set<std::string> const& keysToIgnore = {});

    // ... Accessors
    T const&
    operator()() const
    {
      return *value_;
    }

    ParameterSet const&
    get_PSet() const
    {
      return TableBase::get_pset();
    }

    //=====================================================
    // Expert-only
    using default_type = T;
    using value_type = T;

  private:
    std::shared_ptr<T> value_{std::make_shared<T>()};
  };

  //=====================================================
  // Implementation

  template <table_or_atom_compatible T, keys_to_ignore_provider... KeysToIgnore>
  template <typename... TCARGS>
  Table<T, KeysToIgnore...>::Table(Name&& name, TCARGS&&... tcargs)
    : Table{std::move(name), Comment(""), std::forward<TCARGS>(tcargs)...}
  {}

  template <table_or_atom_compatible T, keys_to_ignore_provider... KeysToIgnore>
  template <typename... TCARGS>
  Table<T, KeysToIgnore...>::Table(Name&& name,
                                   Comment&& comment,
                                   TCARGS&&... tcargs)
    : TableBase{std::move(name),
                std::move(comment),
                par_style::REQUIRED,
                detail::AlwaysUse}
    , RegisterIfTableMember{this}
    , value_{std::make_shared<T>(std::forward<TCARGS>(tcargs)...)}
  {
    finalize_members();
    maybe_implicitly_default();
    NameStackRegistry::end_of_ctor();
  }

  template <table_or_atom_compatible T, keys_to_ignore_provider... KeysToIgnore>
  template <fhicl::maybe_use_param F, typename... TCARGS>
  Table<T, KeysToIgnore...>::Table(Name&& name,
                                   Comment&& comment,
                                   F maybeUse,
                                   TCARGS&&... tcargs)
    : TableBase{std::move(name),
                std::move(comment),
                par_style::REQUIRED_CONDITIONAL,
                maybeUse}
    , RegisterIfTableMember{this}
    , value_{std::make_shared<T>(std::forward<TCARGS>(tcargs)...)}
  {
    finalize_members();
    maybe_implicitly_default();
    NameStackRegistry::end_of_ctor();
  }

  template <table_or_atom_compatible T, keys_to_ignore_provider... KeysToIgnore>
  Table<T, KeysToIgnore...>::Table(ParameterSet const& pset,
                                   std::set<std::string> const& keysToIgnore)
    : TableBase{Name("<top_level>"),
                Comment(""),
                par_style::REQUIRED,
                detail::AlwaysUse}
    , RegisterIfTableMember{this}
  {
    finalize_members();
    std::set<std::string> result{keysToIgnore};
    detail::concatenate_keys(result, KeysToIgnore{}()...);
    maybe_implicitly_default();
    NameStackRegistry::end_of_ctor();
    validate(pset, result);
  }

}

#endif /* fhiclcpp_types_Table_h */

// Local variables:
// mode: c++
// End:
