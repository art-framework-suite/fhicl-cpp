#ifndef fhiclcpp_ParameterSetRegistry_h
#define fhiclcpp_ParameterSetRegistry_h

// ======================================================================
//
// ParameterSetRegistry
//
// ======================================================================

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/ParameterSetID.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/fwd.h"

#include <concepts>
#include <mutex>
#include <unordered_map>

struct sqlite3;
struct sqlite3_stmt;

namespace fhicl {

  class ParameterSetRegistry;

  namespace detail {
    class HashParameterSetID;
    void throwOnSQLiteFailure(int rc, char* msg = nullptr);
    void throwOnSQLiteFailure(sqlite3* db, char* msg = nullptr);

    template <typename FwdIt, typename ValueType>
    concept referent_matches =
      std::same_as<typename std::iterator_traits<FwdIt>::value_type, ValueType>;
  }
}

class fhicl::detail::HashParameterSetID {
public:
  size_t operator()(ParameterSetID const& id) const;

private:
  std::hash<std::string> hash_;
};

class fhicl::ParameterSetRegistry {
public:
  ParameterSetRegistry(ParameterSet const&) = delete;
  ParameterSetRegistry(ParameterSet&&) = delete;
  ParameterSetRegistry& operator=(ParameterSet const&) = delete;
  ParameterSetRegistry& operator=(ParameterSet&&) = delete;
  ~ParameterSetRegistry();

  // Type aliases.
  using collection_type = std::
    unordered_map<ParameterSetID, ParameterSet, detail::HashParameterSetID>;
  using key_type = collection_type::key_type;
  using mapped_type = collection_type::mapped_type;
  using value_type = collection_type::value_type;
  using size_type = collection_type::size_type;
  using const_iterator = collection_type::const_iterator;

  // DB interaction.
  static void importFrom(sqlite3* db);
  static void exportTo(sqlite3* db);
  static void stageIn();

  // Observers.
  static bool empty();
  static size_type size();

  // Put:
  // 1. A single ParameterSet.
  static ParameterSetID const& put(ParameterSet const& ps);
  // 2. A range of iterator to ParameterSet.
  template <detail::referent_matches<mapped_type> FwdIt>
  static void put(FwdIt begin, FwdIt end);
  // 3. A range of iterator to pair<ParameterSetID, ParameterSet>. For
  // each pair, first == second.id() is a prerequisite.
  template <detail::referent_matches<value_type> FwdIt>
  static void put(FwdIt begin, FwdIt end);
  // 4. A collection_type. For each value_type, first == second.id() is
  // a prerequisite.
  static void put(collection_type const& c);

  // Accessors.
  static collection_type const& get() noexcept;
  static ParameterSet const& get(ParameterSetID const& id);
  static bool get(ParameterSetID const& id, ParameterSet& ps);
  static bool has(ParameterSetID const& id);

private:
  ParameterSetRegistry();
  static ParameterSetRegistry& instance_();
  const_iterator find_(ParameterSetID const& id);

  sqlite3* primaryDB_;
  sqlite3_stmt* stmt_{nullptr};
  collection_type registry_{};
  static std::recursive_mutex mutex_;
};

inline bool
fhicl::ParameterSetRegistry::empty()
{
  std::lock_guard sentry{mutex_};
  return instance_().registry_.empty();
}

inline auto
fhicl::ParameterSetRegistry::size() -> size_type
{
  std::lock_guard sentry{mutex_};
  return instance_().registry_.size();
}

// 1.
inline auto
fhicl::ParameterSetRegistry::put(ParameterSet const& ps)
  -> ParameterSetID const&
{
  std::lock_guard sentry{mutex_};
  return instance_().registry_.emplace(ps.id(), ps).first->first;
}

// 2.
template <fhicl::detail::referent_matches<
  fhicl::ParameterSetRegistry::mapped_type> FwdIt>
inline auto
fhicl::ParameterSetRegistry::put(FwdIt b, FwdIt const e) -> void
{
  // No lock here -- it will be acquired by 3.
  for (; b != e; ++b) {
    (void)put(*b);
  }
}

// 3.
template <fhicl::detail::referent_matches<
  fhicl::ParameterSetRegistry::value_type> FwdIt>
inline auto
fhicl::ParameterSetRegistry::put(FwdIt const b, FwdIt const e) -> void
{
  std::lock_guard sentry{mutex_};
  instance_().registry_.insert(b, e);
}

// 4.
inline void
fhicl::ParameterSetRegistry::put(collection_type const& c)
{
  // No lock here -- it will be acquired by 3.
  put(c.cbegin(), c.cend());
}

inline auto
fhicl::ParameterSetRegistry::get() noexcept -> collection_type const&
{
  std::lock_guard sentry{mutex_};
  return instance_().registry_;
}

inline auto
fhicl::ParameterSetRegistry::get(ParameterSetID const& id)
  -> ParameterSet const&
{
  std::lock_guard sentry{mutex_};
  auto it = instance_().find_(id);
  if (it == instance_().registry_.cend()) {
    throw exception(error::cant_find, "Can't find ParameterSet")
      << "with ID " << id.to_string() << " in the registry.";
  }
  return it->second;
}

inline bool
fhicl::ParameterSetRegistry::get(ParameterSetID const& id, ParameterSet& ps)
{
  std::lock_guard sentry{mutex_};
  bool result{false};
  auto it = instance_().find_(id);
  if (it != instance_().registry_.cend()) {
    ps = it->second;
    result = true;
  }
  return result;
}

inline bool
fhicl::ParameterSetRegistry::has(ParameterSetID const& id)
{
  std::lock_guard sentry{mutex_};
  auto const& reg = instance_().registry_;
  return reg.find(id) != reg.cend();
}

inline auto
fhicl::ParameterSetRegistry::instance_() -> ParameterSetRegistry&
{
  static ParameterSetRegistry s_registry;
  return s_registry;
}

inline size_t
fhicl::detail::HashParameterSetID::operator()(ParameterSetID const& id) const
{
  return hash_(id.to_string());
}

#endif /* fhiclcpp_ParameterSetRegistry_h */

// Local Variables:
// mode: c++
// End:
