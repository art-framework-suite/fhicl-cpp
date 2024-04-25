#ifndef fhiclcpp_types_TableFragment_h
#define fhiclcpp_types_TableFragment_h

/*
  'TableFragment' is an auxiliary class used to support the situation
  where a user has a parameter set that looks like:

     pset: {

        a1 : <value>
        a2 : <value>

        b3 : <value>
        b4 : <value>

     }

  where the "a*" parameters are used by algorithm/class 'A' and the
  "b*" parameters are used by algorithm/class 'B'.  Specifically, it
  allows users to define separate configurations for 'A' and 'B'
  (using shorthand):

     struct A { Atom<T> a1; Atom<T> a2; };
     struct B { Atom<T> b1; Atom<T> b2; };

  with a full configuration that looks like:

     struct Config {
       TableFragment<A> a;
       TableFragment<B> b;
     };

  In this way, the parameters used by 'A' are separated from those
  used by 'B', even though FHiCL configuration for 'pset' is flat.

  A 'TableFragment' is not a fhiclcpp type, per se, since it does not
  represent a meaningful FHiCL construct.  It does not derive from
  ParameterBase, nor is it registered anywhere.
*/

#include "fhiclcpp/type_traits.h"
#include "fhiclcpp/types/detail/type_traits_error_msgs.h"

namespace fhicl {

  //========================================================
  template <table_fragment_compatible T>
  class TableFragment final {
  public:
    // Compiler-produced default c'tor (and friends) is fine since 'T'
    // is of class type.

    auto const&
    operator()() const
    {
      return value_;
    }

    // Expert
    struct fhicl_type_tag {};
    struct fhicl_table_fragment_tag {};

  private:
    T value_;
  };
}

#endif /* fhiclcpp_types_TableFragment_h */

// Local variables:
// mode: c++
// End:
