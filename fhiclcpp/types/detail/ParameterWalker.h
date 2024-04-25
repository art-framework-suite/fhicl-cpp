#ifndef fhiclcpp_types_detail_ParameterWalker_h
#define fhiclcpp_types_detail_ParameterWalker_h

/*
  ======================================================================

  ConstParameterWalker
  ParameterWalker

  ======================================================================

  These classes make it possible to traverse the nested configuration
  for a given parameter.  They are very similar in functionality to
  the 'ParameterSet::walk_' function.

  The tree-walking functionality is provided via derivation:

      class MyWalker : (Const)?ParameterWalker
      {
        // members, functions, etc.
      };

  By inheriting from ConstParameterWalker, the subclass promises not to
  alter the configuration during the walk; inheriting from
  ParameterWalker confers no such obligation.

  Heuristically, it looks like:

      for (auto p : parameters) {
         if (!psw.before_action(p))
             return;

         if (is_table(param)) {
             cast_from(p,t)
         *** psw.enter_table(t)
             descend_into_table ...
             psw.exit_table(t)
         }
         else if (is_sequence(param)) {
             cast_from(p,s)
         *** psw.enter_sequence(s)
             loop_through_sequence ...
             psw.exit_sequence(s)
         }
         else if (is_atom(param)) {
             cast_from(p,a)
         *** psw.atom(a)
         }
         else {
             cast_from(p,dp)
         *** psw.delegated_parameter(dp)
         }

         psw.after_action(p)
      }

  The actions that are to be taken per parameter category (table,
  sequence, or atom) are defined entirely by overrides to the
  ParameterWalkerImpl virtual functions that 'psw' calls (as shown above).

  The function calls prefaced with '***' correspond to pure virtual
  functions, which must have corresponding overrides in any derived
  classes.

  The 'exit_{table,sequence}' functions are provided if (e.g.) the
  derived-class state needs to be restored after the table or sequence
  traversal.  The '{before,after}_action' virtual functions are
  provided so that category-agnostic instructions can be executed
  before or after the category-specific ones.

  The required and optional member functions to be overridden by
  subclasses of ParameterWalker and ConstParameterWalker are shown
  below:

  class ConstParameterWalker {
  ⋮
   virtual bool before_action(ParameterBase const&);
   virtual void after_action(ParameterBase const&);

   virtual void atom(AtomBase const&) = 0;
   virtual void delegatedParameter(DelegatedBase const&) = 0;
   virtual void enter_table(TableBase const&) = 0;
   virtual void exit_table(TableBase const&);
   virtual void enter_sequence(SequenceBase const&) = 0;
   virtual void exit_sequence(SequenceBase const&);
  ⋮
  };

  class ParameterWalker {
  ⋮
   virtual bool before_action(ParameterBase&);
   virtual void after_action(ParameterBase&);

   virtual void atom(AtomBase&) = 0;
   virtual void delegatedParameter(DelegatedBase&) = 0;
   virtual void enter_table(TableBase&) = 0;
   virtual void exit_table(TableBase&);
   virtual void enter_sequence(SequenceBase&) = 0;
   virtual void exit_sequence(SequenceBase&);
  ⋮
  };

*/

#include "fhiclcpp/types/detail/ParameterWalkerImpl.h"

namespace fhicl::detail {
  using ParameterWalker =
    ParameterWalkerImpl<tt::const_flavor::require_non_const>;
  using ConstParameterWalker =
    ParameterWalkerImpl<tt::const_flavor::require_const>;
}

#endif /* fhiclcpp_types_detail_ParameterWalker_h */

// Local variables:
// mode: c++
// End:
