#ifndef fhiclcpp_types_OptionalTupleAs_h
#define fhiclcpp_types_OptionalTupleAs_h

#include "fhiclcpp/type_traits.h"
#include "fhiclcpp/types/ConfigPredicate.h"
#include "fhiclcpp/types/OptionalTuple.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/ParameterWalker.h"
#include "fhiclcpp/types/detail/type_traits_error_msgs.h"

#include <memory>
#include <string>
#include <tuple>
#include <utility>

namespace fhicl {

  //==================================================================
  // e.g. OptionalTupleAs<T,int,double,bool> ====> T(int,double,bool)
  //
  template <typename T, typename... ARGS>
  class OptionalTupleAs;

  template <typename T, typename... ARGS>
  class OptionalTupleAs<T(ARGS...)> {
  public:
    explicit OptionalTupleAs(Name&& name);
    explicit OptionalTupleAs(Name&& name, Comment&& comment);
    template <fhicl::maybe_use_param F>
    explicit OptionalTupleAs(Name&& name, Comment&& comment, F maybeUse);

    std::optional<T>
    operator()() const
    {
      if (auto via = tupleObj_()) {
        return make_optional(std::make_from_tuple(*via));
      }
      return std::nullopt;
    }

    // Obsolete
    bool
    operator()(T& result) const
    {
      auto t = operator()();
      if (t) {
        result = *t;
      }
      return t.has_value();
    }

    bool
    hasValue() const
    {
      return tupleObj_.hasValue();
    }

    // Expert
    struct fhicl_type_tag {};
    struct fhicl_optional_tag {};

  private:
    OptionalTuple<ARGS...> tupleObj_;

    Comment
    conversion_comment(Comment&& comment) const
    {
      std::string const preface =
        "N.B. The following sequence is converted to type:";
      std::string const name =
        "        '" + cet::demangle_symbol(typeid(T).name()) + "'";
      std::string const user_comment =
        comment.value.empty() ? "" : "\n\n" + comment.value;

      std::ostringstream oss;
      oss << preface << '\n' << name << user_comment;

      return Comment{oss.str().c_str()};
    }
  };

  //==================================================================
  // IMPLEMENTATION

  template <typename T, typename... ARGS>
  OptionalTupleAs<T(ARGS...)>::OptionalTupleAs(Name&& name)
    : OptionalTupleAs{std::move(name), Comment("")}
  {}

  template <typename T, typename... ARGS>
  OptionalTupleAs<T(ARGS...)>::OptionalTupleAs(Name&& name, Comment&& comment)
    : tupleObj_{std::move(name), conversion_comment(std::move(comment))}
  {}

  template <typename T, typename... ARGS>
  template <fhicl::maybe_use_param F>
  OptionalTupleAs<T(ARGS...)>::OptionalTupleAs(Name&& name,
                                               Comment&& comment,
                                               F maybeUse)
    : tupleObj_{std::move(name),
                conversion_comment(std::move(comment)),
                maybeUse}
  {}
}

#endif /* fhiclcpp_types_OptionalTupleAs_h */

// Local variables:
// mode: c++
// End:
