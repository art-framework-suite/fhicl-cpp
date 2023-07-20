#ifndef fhiclcpp_types_detail_SequenceBase_h
#define fhiclcpp_types_detail_SequenceBase_h

#include "fhiclcpp/types/detail/PW_fwd.h"
#include "fhiclcpp/types/detail/ParameterBase.h"

#include <array>
#include <tuple>
#include <vector>

namespace fhicl::detail {

  //========================================================
  class SequenceBase : public ParameterBase {
  public:
    template <fhicl::maybe_use_param F>
    SequenceBase(Name&& name,
                 Comment&& comment,
                 par_style const vt,
                 par_type const type,
                 F maybeUse)
      : ParameterBase{name, comment, vt, type, maybeUse}
    {}

  protected:
    using ConstParameterWalker = ParameterWalkerImpl<tt::const_flavor::require_const>;
    using ParameterWalker = ParameterWalkerImpl<tt::const_flavor::require_non_const>;

  public:
    bool
    empty() const noexcept
    {
      return size() == 0;
    }
    std::size_t
    size() const noexcept
    {
      return get_size();
    }

    void
    prepare_elements_for_validation(std::size_t const n)
    {
      do_prepare_elements_for_validation(n);
    }
    void
    walk_elements(ParameterWalker& pw)
    {
      do_walk_elements(pw);
    }
    void
    walk_elements(ConstParameterWalker& pw) const
    {
      do_walk_elements(pw);
    }

  private:
    virtual std::size_t get_size() const noexcept = 0;

    virtual void do_prepare_elements_for_validation(std::size_t) = 0;
    virtual void do_walk_elements(ParameterWalker&) = 0;
    virtual void do_walk_elements(ConstParameterWalker&) const = 0;
  };
}

#endif /* fhiclcpp_types_detail_SequenceBase_h */

// Local variables:
// mode: c++
// End:
