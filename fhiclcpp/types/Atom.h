#ifndef fhiclcpp_types_Atom_h
#define fhiclcpp_types_Atom_h

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/type_traits.h"
#include "fhiclcpp/types/Comment.h"
#include "fhiclcpp/types/ConfigPredicate.h"
#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/detail/AtomBase.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterMetadata.h"
#include "fhiclcpp/types/detail/TableMemberRegistry.h"
#include "fhiclcpp/types/detail/ostream_helpers.h"
#include "fhiclcpp/types/detail/type_traits_error_msgs.h"

#include <memory>
#include <sstream>
#include <string>

namespace fhicl {

  //========================================================
  template <table_or_atom_compatible T>
  class Atom final : public detail::AtomBase,
                     private detail::RegisterIfTableMember {
  public:
    
    //=====================================================
    // User-friendly
    // ... c'tors
    explicit Atom(Name&& name);
    explicit Atom(Name&& name, Comment&& comment);
    template <fhicl::maybe_use_param F>
    explicit Atom(Name&& name, Comment&& comment, F useIf);

    // ... c'tors supporting defaults
    explicit Atom(Name&& name, T const& dflt_value);
    explicit Atom(Name&& name, Comment&& comment, T const& dflt_value);
    template <fhicl::maybe_use_param F>
    explicit Atom(Name&& name, Comment&& comment, F useIf, T const& dflt_value);

    // ... Accessors
    auto const&
    operator()() const
    {
      return value_;
    }

    // Expert-only
    using default_type = T;
    using value_type = T;

  private:
    value_type value_{};

    std::string get_stringified_value() const override;
    void do_set_value(fhicl::ParameterSet const& pset) override;
  };
}

// Implementation
#include "fhiclcpp/detail/printing_helpers.h"
#include "fhiclcpp/types/detail/strip_containing_names.h"

namespace fhicl {

  template <table_or_atom_compatible T>
  Atom<T>::Atom(Name&& name, Comment&& comment)
    : AtomBase{std::move(name),
               std::move(comment),
               par_style::REQUIRED,
               detail::AlwaysUse}
    , RegisterIfTableMember{this}
  {
    NameStackRegistry::end_of_ctor();
  }

  template <table_or_atom_compatible T>
  template <fhicl::maybe_use_param F>
  Atom<T>::Atom(Name&& name, Comment&& comment, F maybeUse)
    : AtomBase{std::move(name),
               std::move(comment),
               par_style::REQUIRED_CONDITIONAL,
               maybeUse}
    , RegisterIfTableMember{this}
  {
    NameStackRegistry::end_of_ctor();
  }

  template <table_or_atom_compatible T>
  Atom<T>::Atom(Name&& name, Comment&& comment, T const& dflt_value)
    : AtomBase{std::move(name),
               std::move(comment),
               par_style::DEFAULT,
               detail::AlwaysUse}
    , RegisterIfTableMember{this}
    , value_{dflt_value}
  {
    NameStackRegistry::end_of_ctor();
  }

  template <table_or_atom_compatible T>
  template <maybe_use_param F>
  Atom<T>::Atom(Name&& name, Comment&& comment, F maybeUse, T const& dflt_value)
    : AtomBase{std::move(name),
               std::move(comment),
               par_style::DEFAULT_CONDITIONAL,
               maybeUse}
    , RegisterIfTableMember{this}
    , value_{dflt_value}
  {
    NameStackRegistry::end_of_ctor();
  }

  template <table_or_atom_compatible T>
  Atom<T>::Atom(Name&& name) : Atom{std::move(name), Comment("")}
  {}

  template <table_or_atom_compatible T>
  Atom<T>::Atom(Name&& name, T const& dflt_value)
    : Atom{std::move(name), Comment(""), dflt_value}
  {}

  template <table_or_atom_compatible T>
  std::string
  Atom<T>::get_stringified_value() const
  {
    std::stringstream oss;
    if (has_default()) {
      using namespace detail::yes_defaults;
      oss << maybe_quotes<T>(value_);
    } else {
      using namespace detail::no_defaults;
      oss << expected_types<T>();
    }
    return oss.str();
  }

  template <table_or_atom_compatible T>
  void
  Atom<T>::do_set_value(fhicl::ParameterSet const& pset)
  {
    auto const trimmed_key = detail::strip_first_containing_name(key());
    if (has_default()) {
      // Override default value if the key is present
      pset.get_if_present<T>(trimmed_key, value_);
    } else
      value_ = pset.get<T>(trimmed_key);
  }
}

#endif /* fhiclcpp_types_Atom_h */

// Local variables:
// mode: c++
// End:
