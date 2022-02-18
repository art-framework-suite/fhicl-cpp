#ifndef fhiclcpp_types_detail_TableMemberRegistry_h
#define fhiclcpp_types_detail_TableMemberRegistry_h

// ========================================================================
// 'TableMemberRegistry' exists solely because C++ does not have the
// reflection facilities required to return the members of a struct or
// class, which is needed to walk through the members of a table.  For
// example, consider the following struct and associated Table<S>
// object:
//
//   struct S {
//     Atom<int> num{Name("num")};
//     Sequence<int> list{Name("list")};
//   };
//   Table<S> t{Name("t")};
//
// C++ does not have the ability to return the list of data members
// for the struct 'S'.  The registration system below, however, does
// do this by taking the Name values above ("num" and "list"), and
// emplacing them into a container corresponding to the table members
// associated with 't'.
//
// The way this registry is used is via private inheritance of a
// 'RegisterIfTableMember' auxiliary class.  Each fhiclcpp parameter
// type (Atom, Sequence, etc.), inherits from the auxiliary class.
//
// If the C++ reflection facilities improve to the level that a
// struct's or class's members can be returned (either at
// compile-time, or run-time), this registry should be removed.
// =============================================================================

#include "cetlib/exempt_ptr.h"
#include "fhiclcpp/fwd.h"
#include "fhiclcpp/types/detail/ParameterBase.h"

#include <stack>
#include <vector>

namespace fhicl::detail {

  class RegisterIfTableMember;
  class TableBase;

  // All-private class that only fhicl::detail::RegisterIfTableMember
  // and fhicl::detail::TableBase can access via friendship.

  class TableMemberRegistry {

    using base_ptr = cet::exempt_ptr<ParameterBase>;
    using table_members_t = std::vector<base_ptr>;
    std::stack<table_members_t> tables_;

    static TableMemberRegistry& instance_();

    // Retrieval facilities for fhicl::(Optional)Table
    friend class TableBase;

    static std::vector<base_ptr> release_members();

    // Registration facilities
    friend class RegisterIfTableMember;

    static void emplace_table_member(ParameterBase* pb);
    static void new_table();
  };

  //========================================================
  class RegisterIfTableMember {
  public:
    explicit RegisterIfTableMember(ParameterBase* pb);
  };
} // namespace fhicl::detail

#endif /* fhiclcpp_types_detail_TableMemberRegistry_h */

// Local variables:
// mode: c++
// End:
