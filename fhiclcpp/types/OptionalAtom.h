#ifndef fhiclcpp_types_OptionalAtom_h
#define fhiclcpp_types_OptionalAtom_h

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

#include <optional>
#include <sstream>
#include <string>

namespace fhicl {

  //========================================================
  template <table_or_atom_compatible T>
  class OptionalAtom final : public detail::AtomBase,
                             private detail::RegisterIfTableMember {
  public:

    //=====================================================
    // User-friendly
    // ... c'tors
    explicit OptionalAtom(Name&& name);
    explicit OptionalAtom(Name&& name, Comment&& comment);
    template <fhicl::maybe_use_param F>
    explicit OptionalAtom(Name&& name, Comment&& comment, F maybeUse);

    // ... Accessors
    std::optional<T>
    operator()() const
    {
      return value_;
    }

    // Obsolete
    bool
    operator()(T& value) const
    {
      if (hasValue()) {
        value = *value_;
        return true;
      }
      return false;
    }

    bool
    hasValue() const
    {
      return value_.has_value();
    }

    // Expert-only
    struct fhicl_optional_tag {};
    using value_type = T;

    OptionalAtom();

  private:
    std::optional<T> value_{};

    std::string get_stringified_value() const override;
    void do_set_value(fhicl::ParameterSet const&) override;
  };
}

#include "fhiclcpp/detail/printing_helpers.h"
#include "fhiclcpp/types/detail/strip_containing_names.h"

namespace fhicl {

  template <table_or_atom_compatible T>
  OptionalAtom<T>::OptionalAtom(Name&& name)
    : OptionalAtom{std::move(name), Comment("")}
  {}

  template <table_or_atom_compatible T>
  OptionalAtom<T>::OptionalAtom(Name&& name, Comment&& comment)
    : AtomBase{std::move(name),
               std::move(comment),
               par_style::OPTIONAL,
               detail::AlwaysUse}
    , RegisterIfTableMember{this}
  {
    NameStackRegistry::end_of_ctor();
  }

  template <table_or_atom_compatible T>
  template <maybe_use_param F>
  OptionalAtom<T>::OptionalAtom(Name&& name, Comment&& comment, F maybeUse)
    : AtomBase{std::move(name),
               std::move(comment),
               par_style::OPTIONAL_CONDITIONAL,
               maybeUse}
    , RegisterIfTableMember{this}
  {
    NameStackRegistry::end_of_ctor();
  }

  template <table_or_atom_compatible T>
  std::string
  OptionalAtom<T>::get_stringified_value() const
  {
    std::stringstream oss;
    if (value_) {
      using namespace detail::yes_defaults;
      oss << maybe_quotes<T>(*value_);
    } else {
      using namespace detail::no_defaults;
      oss << expected_types<T>();
    }
    return oss.str();
  }

  template <table_or_atom_compatible T>
  void
  OptionalAtom<T>::do_set_value(fhicl::ParameterSet const& pset)
  {
    auto const trimmed_key = detail::strip_first_containing_name(key());
    if (auto value = pset.get_if_present<T>(trimmed_key)) {
      value_ = *value;
    }
  }
}

#endif /* fhiclcpp_types_OptionalAtom_h */

// Local variables:
// mode: c++
// End:
