#ifndef fhiclcpp_types_detail_per_thread_holder_h
#define fhiclcpp_types_detail_per_thread_holder_h

#include "tbb/concurrent_hash_map.h"

#include <thread>

namespace fhicl::detail {
  struct thread_hash_compare {
    static size_t
    hash(std::thread::id id)
    {
      static std::hash<std::thread::id> const hasher{};
      return hasher(id);
    }
    static bool
    equal(std::thread::id id1, std::thread::id id2)
    {
      return id1 == id2;
    }
  };
  template <typename T>
  class per_thread_holder {
    using registry_t =
      tbb::concurrent_hash_map<std::thread::id, T, thread_hash_compare>;
    using accessor = typename registry_t::accessor;

  public:
    T&
    slot_for_current_thread()
    {
      return slot_for(std::this_thread::get_id());
    }

  private:
    T&
    slot_for(std::thread::id id)
    {
      // Lock held on key's map entry until the function returns.
      accessor access_token;
      if (not registry_.insert(access_token, id)) {
        // Entry already exists; return cached entry.
        return access_token->second;
      }

      access_token->second = T{};
      return access_token->second;
    }

    registry_t registry_;
  };
}

#endif /* fhiclcpp_types_detail_per_thread_holder_h */

// Local Variables:
// mode: c++
// End:
