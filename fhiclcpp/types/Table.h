#ifndef fhiclcpp_types_Table_h
#define fhiclcpp_types_Table_h

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/type_traits.h"
#include "fhiclcpp/types/Comment.h"
#include "fhiclcpp/types/ConfigPredicate.h"
#include "fhiclcpp/types/MaybeUseFunction.h"
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
  template <typename T, typename KeysToIgnore = void>
  class Table final : public detail::TableBase,
                      private detail::RegisterIfTableMember {
  public:
    static_assert(!tt::is_sequence_type_v<T>, NO_STD_CONTAINERS);
    static_assert(!tt::is_fhicl_type_v<T>, NO_NESTED_FHICL_TYPES_IN_TABLE);
    static_assert(!tt::is_table_fragment_v<T>, NO_NESTED_TABLE_FRAGMENTS);
    static_assert(!tt::is_delegated_parameter_v<T>, NO_DELEGATED_PARAMETERS);

    //=====================================================
    // User-friendly
    // ... c'tors
    template <typename... TCARGS>
    explicit Table(Name&& name, TCARGS&&... tcargs);
    template <typename... TCARGS>
    explicit Table(Name&& name, Comment&& comment, TCARGS&&... tcargs);
    template <typename... TCARGS>
    explicit Table(Name&& name,
                   Comment&& comment,
                   MaybeUseFunction maybeUse,
                   TCARGS&&... tcargs);

    // Choose this c'tor if user specifies the second 'KeysToIgnore' template
    // argument.
    template <typename K = KeysToIgnore,
              typename = std::enable_if_t<tt::is_callable<K>::value>>
    Table(ParameterSet const& pset);

    // If user does not specify the second 'KeysToIgnore' template argument,
    // then choose this c'tor.
    template <typename K = KeysToIgnore,
              typename = std::enable_if_t<!tt::is_callable<K>::value>>
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

    struct Impl {};
    Table(ParameterSet const&, std::set<std::string> const&, Impl);
  };

  //=====================================================
  // Implementation

  template <typename T, typename KeysToIgnore>
  template <typename... TCARGS>
  Table<T, KeysToIgnore>::Table(Name&& name, TCARGS&&... tcargs)
    : Table{std::move(name), Comment(""), std::forward<TCARGS>(tcargs)...}
  {}

  template <typename T, typename KeysToIgnore>
  template <typename... TCARGS>
  Table<T, KeysToIgnore>::Table(Name&& name,
                                Comment&& comment,
                                TCARGS&&... tcargs)
    : TableBase{std::move(name),
                std::move(comment),
                par_style::REQUIRED,
                detail::AlwaysUse()}
    , RegisterIfTableMember{this}
    , value_{std::make_shared<T>(std::forward<TCARGS>(tcargs)...)}
  {
    finalize_members();
    maybe_implicitly_default();
    NameStackRegistry::end_of_ctor();
  }

  template <typename T, typename KeysToIgnore>
  template <typename... TCARGS>
  Table<T, KeysToIgnore>::Table(Name&& name,
                                Comment&& comment,
                                MaybeUseFunction maybeUse,
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

  template <typename T, typename KeysToIgnore>
  template <typename, typename>
  Table<T, KeysToIgnore>::Table(ParameterSet const& pset)
    : Table{pset, KeysToIgnore{}(), Impl{}}
  {}

  template <typename T, typename KeysToIgnore>
  template <typename, typename>
  Table<T, KeysToIgnore>::Table(ParameterSet const& pset,
                                std::set<std::string> const& keysToIgnore)
    : Table{pset, keysToIgnore, Impl{}}
  {}

  template <typename T, typename KeysToIgnore>
  Table<T, KeysToIgnore>::Table(ParameterSet const& pset,
                                std::set<std::string> const& keysToIgnore,
                                Impl)
    : TableBase{Name("<top_level>"),
                Comment(""),
                par_style::REQUIRED,
                detail::AlwaysUse()}
    , RegisterIfTableMember{this}
  {
    finalize_members();
    maybe_implicitly_default();
    NameStackRegistry::end_of_ctor();
    validate(pset, keysToIgnore);
  }

}

#endif /* fhiclcpp_types_Table_h */

// Local variables:
// mode: c++
// End:
