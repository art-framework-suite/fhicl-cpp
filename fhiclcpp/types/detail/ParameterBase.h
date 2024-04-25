#ifndef fhiclcpp_types_detail_ParameterBase_h
#define fhiclcpp_types_detail_ParameterBase_h

// clang-format off
/*
  ParameterBase is the most fundamental base class for all fhiclcpp
  types.  The taxonomy is:

          ParameterBase
         /      |    \ \____________________________________
        /       |     \___________________                  \
       /        |                         \                  \
      /         |                          \                  \
  AtomBase   TableBase                  SequenceBase         DelegateBase
     |          |                      /      |     \             \
     |          |                     /       |      \             \
  Atom<T>    Table<T>   Sequence<T,SZ>  Sequence<T>   Tuple<T...>   DelegatedParameter


  All concrete Optional* fhiclcpp types also inherit from the
  corresponding base classes (e.g. OptionalAtom<T> inherits from
  AtomBase, not OptionalAtomBase).  The design is meant to closely
  follow the classification of FHiCL values, as described in the FHiCL
  language quick start guide.
*/
// clang-format on

#include "fhiclcpp/fwd.h"
#include "fhiclcpp/type_traits.h"
#include "fhiclcpp/types/ConfigPredicate.h"
#include "fhiclcpp/types/detail/ParameterArgumentTypes.h"
#include "fhiclcpp/types/detail/ParameterMetadata.h"

namespace fhicl::detail {

  //========================================================
  class ParameterBase {
  public:
    template <fhicl::maybe_use_param F>
    ParameterBase(Name const& name,
                  Comment const& comment,
                  par_style const vt,
                  par_type const type,
                  F maybeUse = AlwaysUse);
    virtual ~ParameterBase();

    std::string const& key() const;
    std::string const& name() const;
    std::string const& comment() const;
    bool has_default() const;
    bool is_optional() const;
    bool is_conditional() const;
    par_type parameter_type() const;
    bool should_use() const;

    // Modifiers
    void set_value(fhicl::ParameterSet const& ps);
    bool preset_value(fhicl::ParameterSet const& ps);
    void set_par_style(par_style const vt);

    struct fhicl_type_tag {};

  private:
    virtual bool do_preset_value(fhicl::ParameterSet const&);
    virtual void do_set_value(fhicl::ParameterSet const&) = 0;

    ParameterMetadata mdata_;
    std::function<bool()> maybeUse_;
  };
}

template <fhicl::maybe_use_param F>
fhicl::detail::ParameterBase::ParameterBase(Name const& name,
                                            Comment const& comment,
                                            par_style const vt,
                                            par_type const type,
                                            F maybeUse)
  : mdata_{name, comment, vt, type}, maybeUse_{std::move(maybeUse)}
{}

#endif /* fhiclcpp_types_detail_ParameterBase_h */

// Local variables:
// mode: c++
// End:
