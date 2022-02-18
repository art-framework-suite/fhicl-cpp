#include "fhiclcpp/types/detail/TableMemberRegistry.h"
#include "fhiclcpp/detail/printing_helpers.h"
#include "fhiclcpp/types/detail/per_thread_holder.h"

namespace fhicl::detail {

  TableMemberRegistry&
  TableMemberRegistry::instance_()
  {
    // The use of the registry is restricted to the construction
    // of fhiclcpp types.  As construction happens on only one
    // thread, it is sufficient for each thread to have its own
    // copy.  Although a thread-local static would be appropriate
    // here, not all implementations adequately support
    // thread-local variables for the use case here.  We thus use
    // a custom-built per-thread cache.
    static per_thread_holder<TableMemberRegistry> registry;
    return registry.slot_for_current_thread();
  }

  std::vector<cet::exempt_ptr<ParameterBase>>
  TableMemberRegistry::release_members()
  {
    auto& tables = instance_().tables_;
    auto result = std::exchange(tables.top(), {});
    tables.pop();
    return result;
  }

  void
  TableMemberRegistry::emplace_table_member(ParameterBase* pb)
  {
    instance_().tables_.top().emplace_back(pb);
  }

  void
  TableMemberRegistry::new_table()
  {
    instance_().tables_.emplace();
  }

  RegisterIfTableMember::RegisterIfTableMember(ParameterBase* pb)
  {
    if (is_table_member(pb->key())) {
      TableMemberRegistry::emplace_table_member(pb);
    }
    if (is_table(pb->parameter_type())) {
      TableMemberRegistry::new_table();
    }
  }
}
