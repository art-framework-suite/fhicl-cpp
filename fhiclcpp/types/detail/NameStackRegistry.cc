#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/types/detail/per_thread_holder.h"

#include <numeric>
#include <regex>

namespace {
  std::regex const re_sequence_element{R"(\[\d+\])"};
}

namespace fhicl {
  std::string
  NameStackRegistry::full_key(std::string const& name)
  {
    if (name.empty()) {
      throw exception{error::other, "NameStackRegistry::full_key"}
        << "Cannot insert empty name into 'NameStackRegistry'.\n";
    }

    auto& names = instance_().names_;
    if (names.empty() || std::regex_match(name, re_sequence_element)) {
      names.emplace_back(name);
    } else {
      names.emplace_back("." + name);
    }
    return std::accumulate(names.begin(), names.end(), std::string{});
  }

  void
  NameStackRegistry::end_of_ctor()
  {
    instance_().names_.pop_back();
  }

  void
  NameStackRegistry::clear()
  {
    instance_().names_.clear();
  }

  bool
  NameStackRegistry::empty()
  {
    return instance_().names_.empty();
  }

  std::string
  NameStackRegistry::current()
  {
    return instance_().names_.back();
  }

  NameStackRegistry&
  NameStackRegistry::instance_()
  {
    // The use of the registry is restricted to the construction of
    // fhiclcpp types.  As construction happens on only one thread,
    // it is sufficient for each thread to have its own copy.
    // Although a thread-local static would be appropriate here, not
    // all implementations adequately support thread-local variables
    // for the use case here.  We thus use a custom-built per-thread
    // cache.
    static detail::per_thread_holder<NameStackRegistry> registry;
    return registry.slot_for_current_thread();
  }
}
