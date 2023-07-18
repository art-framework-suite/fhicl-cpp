#ifndef fhiclcpp_types_TupleAs_h
#define fhiclcpp_types_TupleAs_h

#include "fhiclcpp/type_traits.h"
#include "fhiclcpp/types/ConfigPredicate.h"
#include "fhiclcpp/types/OptionalTuple.h"
#include "fhiclcpp/types/Tuple.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/maybe_insert.h"
#include "fhiclcpp/types/detail/type_traits_error_msgs.h"

#include <memory>
#include <string>
#include <tuple>
#include <utility>

namespace fhicl {

  //==================================================================
  // e.g. TupleAs<T,int,double,bool> ====> T(int,double,bool)
  //
  template <typename T, typename... ARGS>
  class TupleAs;

  template <typename T, typename... ARGS>
  class TupleAs<T(ARGS...)> {
  public:
    using default_type = T;
    using via_type = typename OptionalTuple<ARGS...>::value_type;
    using value_type = T;
    using ftype = typename OptionalTuple<ARGS...>::ftype;

    explicit TupleAs(Name&& name);
    explicit TupleAs(Name&& name, Comment&& comment);
    template <fhicl::maybe_use_param F>
    explicit TupleAs(Name&& name, Comment&& comment, F maybeUse);

    // c'tors supporting default values
    explicit TupleAs(Name&& name, T const& t);
    explicit TupleAs(Name&& name, Comment&& comment, T const& t);
    template <fhicl::maybe_use_param F>
    explicit TupleAs(Name&& name, Comment&& comment, F maybeUse, T const& t);

    T
    operator()() const
    {
      via_type via;
      return tupleObj_(via) ? std::make_from_tuple<T>(via) :
             tupleObj_.has_default() ?
                              *t_ :
                              throw fhicl::exception(
                                cant_find); // fix this exception category!
    }

    //=================================================================
    // expert only

    operator detail::ParameterBase&()
    {
      return tupleObj_;
    } // Allows implicit conversion from
      // TupleAs to ParameterBase (necessary
      // for ParameterWalker)
  private:
    OptionalTuple<ARGS...> tupleObj_;
    std::shared_ptr<T>
      t_{}; // shared instead of unique to allowed Sequence<TupleAs<>> objects.

    Comment conversion_comment(Comment&& comment) const;
    Comment conversion_comment(Comment&& comment, T const& t) const;
  };

  //==================================================================
  // IMPLEMENTATION

  template <typename T, typename... ARGS>
  TupleAs<T(ARGS...)>::TupleAs(Name&& name)
    : TupleAs{std::move(name), Comment("")}
  {}

  template <typename T, typename... ARGS>
  TupleAs<T(ARGS...)>::TupleAs(Name&& name, Comment&& comment)
    : tupleObj_{std::move(name), conversion_comment(std::move(comment))}
  {
    tupleObj_.set_par_style(par_style::REQUIRED);
  }

  template <typename T, typename... ARGS>
  template <fhicl::maybe_use_param F>
  TupleAs<T(ARGS...)>::TupleAs(Name&& name, Comment&& comment, F maybeUse)
    : tupleObj_{std::move(name),
                conversion_comment(std::move(comment)),
                maybeUse}
  {
    tupleObj_.set_par_style(par_style::REQUIRED_CONDITIONAL);
  }

  // c'tors supporting default values
  template <typename T, typename... ARGS>
  TupleAs<T(ARGS...)>::TupleAs(Name&& name, T const& t)
    : TupleAs{std::move(name), Comment(""), t}
  {}

  template <typename T, typename... ARGS>
  TupleAs<T(ARGS...)>::TupleAs(Name&& name, Comment&& comment, T const& t)
    : tupleObj_{std::move(name), conversion_comment(std::move(comment), t)}
    , t_{std::make_shared<T>(t)}
  {
    tupleObj_.set_par_style(par_style::DEFAULT);
  }

  template <typename T, typename... ARGS>
  template <fhicl::maybe_use_param F>
  TupleAs<T(ARGS...)>::TupleAs(Name&& name,
                               Comment&& comment,
                               F maybeUse,
                               T const& t)
    : tupleObj_{std::move(name),
                conversion_comment(std::move(comment), t),
                maybeUse}
    , t_{std::make_shared<T>(t)}
  {
    tupleObj_.set_par_style(par_style::DEFAULT_CONDITIONAL);
  }

  //===============================================================================

  template <typename T, typename... ARGS>
  Comment
  TupleAs<T(ARGS...)>::conversion_comment(Comment&& comment) const
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

  template <typename T, typename... ARGS>
  Comment
  TupleAs<T(ARGS...)>::conversion_comment(Comment&& comment, T const& t) const
  {
    std::string const preface{
      "N.B. The following sequence is converted to type:"};
    std::string const name{"        '" +
                           cet::demangle_symbol(typeid(T).name()) + "'"};

    std::string const user_comment{
      comment.value.empty() ? "" : "\n\n" + comment.value};

    std::ostringstream oss;
    oss << preface << '\n'
        << name << '\n'
        << detail::maybe_insert(t) << user_comment;
    return Comment{oss.str().c_str()};
  }
}

#endif /* fhiclcpp_types_TupleAs_h */

// Local variables:
// mode: c++
// End:
