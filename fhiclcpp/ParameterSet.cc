// ======================================================================
//
// ParameterSet
//
// ======================================================================

#include "fhiclcpp/ParameterSet.h"
#include "cetlib/container_algorithms.h"
#include "fhiclcpp/ParameterSetRegistry.h"
#include "fhiclcpp/ParameterSetWalker.h"
#include "fhiclcpp/detail/KeyAssembler.h"
#include "fhiclcpp/detail/Prettifier.h"
#include "fhiclcpp/detail/PrettifierAnnotated.h"
#include "fhiclcpp/detail/PrettifierPrefixAnnotated.h"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/parse.h"

#include <cstddef>
#include <stack>

using namespace fhicl;
using namespace fhicl::detail;
using namespace std;
using namespace std::string_literals;

using std::any;
using std::any_cast;

using ps_atom_t = ParameterSet::ps_atom_t;
using ps_sequence_t = ParameterSet::ps_sequence_t;
using table_t = intermediate_table::table_t;
using ldbl = long double;

// ======================================================================

namespace {

  // See notes for 'put' specialization below
  void
  fill_src_info(extended_value const& value,
                std::string const& key,
                ParameterSet::annot_t& src_map)
  {
    src_map[key] = value.src_info;
    if (!value.is_a(SEQUENCE))
      return;
    std::size_t i{};
    for (auto const& xval : extended_value::sequence_t(value)) {
      std::ostringstream oss;
      oss << key << "[" << i++ << "]";
      src_map[oss.str()] = xval.src_info;
      fill_src_info(xval, oss.str(), src_map);
    }
  }

  ParameterSet const&
  get_pset_via_any(std::any const& a)
  {
    ParameterSetID const& psid = std::any_cast<ParameterSetID>(a);
    return ParameterSetRegistry::get(psid);
  }
}

// ----------------------------------------------------------------------

fhicl::ParameterSet
fhicl::ParameterSet::make(intermediate_table const& tbl)
{
  ParameterSet result;
  for (auto const& [key, value] : tbl) {
    if (!value.in_prolog)
      result.put(key, value);
  }
  return result;
}

// ----------------------------------------------------------------------

fhicl::ParameterSet
fhicl::ParameterSet::make(extended_value const& xval)
{
  if (!xval.is_a(TABLE))
    throw fhicl::exception(type_mismatch, "extended value not a table");

  ParameterSet result;
  auto const& tbl = table_t(xval);
  for (auto const& [key, value] : tbl) {
    if (!value.in_prolog)
      result.put(key, value);
  }
  return result;
}

// ----------------------------------------------------------------------

fhicl::ParameterSet
fhicl::ParameterSet::make(std::string const& str)
{
  auto const tbl = parse_document(str);
  return ParameterSet::make(tbl);
}

// ----------------------------------------------------------------------

fhicl::ParameterSet
fhicl::ParameterSet::make(std::string const& filename,
                          cet::filepath_maker& maker)
{
  auto const tbl = parse_document(filename, maker);
  return ParameterSet::make(tbl);
}

// ======================================================================

string
ParameterSet::stringify_(any const& a, bool const compact) const
{
  string result;
  if (is_table(a)) {
    auto const& psid = any_cast<ParameterSetID>(a);
    result = '{' + ParameterSetRegistry::get(psid).to_string() + '}';
    if (compact && result.size() > (5 + ParameterSetID::max_str_size())) {
      // Replace with a reference to the ParameterSetID;
      result = std::string("@id::") + psid.to_string();
    }
  } else if (is_sequence(a)) {
    auto const& seq = any_cast<ps_sequence_t>(a);
    result = '[';
    if (!seq.empty()) {
      result.append(stringify_(*seq.begin(), compact));
      for (auto it = seq.cbegin(), e = seq.cend(); ++it != e;) {
        result.append(1, ',').append(stringify_(*it, compact));
      }
    }
    result.append(1, ']');
  } else { // is_atom(a)
    ps_atom_t const str = any_cast<ps_atom_t>(a);
    result = str == string(9, '\0') ? "@nil" : str;
  }
  return result;
} // stringify_()

// ----------------------------------------------------------------------

bool
ParameterSet::is_empty() const
{
  return mapping_.empty();
}

ParameterSetID
ParameterSet::id() const
{
  if (!id_.is_valid()) {
    id_.reset(*this);
  }
  return id_;
}

string
ParameterSet::to_string_(bool const compact) const
{
  string result;
  if (mapping_.empty()) {
    return result;
  }
  auto it = mapping_.begin();
  result.append(it->first).append(1, ':').append(
    stringify_(it->second, compact));
  for (auto const e = mapping_.end(); ++it != e;) {
    result.append(1, ' ').append(it->first).append(1, ':').append(
      stringify_(it->second, compact));
  }
  return result;
}

vector<string>
ParameterSet::get_names() const
{
  vector<string> keys;
  cet::transform_all(mapping_, std::back_inserter(keys), [](auto const& pr) {
    return pr.first;
  });
  return keys;
}

vector<string>
ParameterSet::get_pset_names() const
{
  vector<string> keys;
  for (auto const& [key, value] : mapping_) {
    if (is_table(value)) {
      keys.push_back(key);
    }
  }
  return keys;
}

vector<string>
ParameterSet::get_all_keys() const
{
  KeyAssembler ka;
  walk(ka);
  return ka.result();
}

bool
ParameterSet::find_one_(std::string const& simple_key) const
{
  auto skey = detail::get_sequence_indices(simple_key);

  auto it = mapping_.find(skey.name());
  if (it == mapping_.end()) {
    return false;
  }

  auto a = it->second;
  return detail::find_an_any(skey.indices().cbegin(), skey.indices().cend(), a);
}

std::optional<ParameterSet>
ParameterSet::descend_(std::vector<std::string> const& names) const
{
  if (empty(names)) {
    return std::make_optional(*this);
  }
  ParameterSet const* p{this};
  std::optional<ParameterSet> result;
  for (auto const& name : names) {
    if (!p->find_one_(name)) {
      return std::nullopt;
    }
    if (!p->is_key_to_table(name)) {
      return std::nullopt;
    }

    if (result = p->get_one_<ParameterSet>(name); not result) {
      return std::nullopt;
    }
    p = &result.value();
  }
  return result;
}

bool
ParameterSet::has_key(std::string const& key) const
{
  auto keys = detail::get_names(key);
  auto ps = descend_(keys.tables());
  return ps ? ps->find_one_(keys.last()) : false;
}

// ----------------------------------------------------------------------

std::string
ParameterSet::get_src_info(std::string const& key) const
{
  auto result = srcMapping_.find(key);
  return result != srcMapping_.cend() ? result->second : "";
}

// ----------------------------------------------------------------------

void
ParameterSet::put(std::string const& key)
{
  put(key, nullptr);
}

void
ParameterSet::put_or_replace(std::string const& key)
{
  put_or_replace(key, nullptr); // Replace with nil is always OK.
}

// ----------------------------------------------------------------------

namespace {
  inline void
  check_put_local_key(std::string const& key)
  {
    if (key.find('.') != std::string::npos) {
      throw fhicl::exception(unimplemented, "putXXX() for nested key.");
    }
  }
}

void
ParameterSet::insert_(string const& key, any const& value)
{
  check_put_local_key(key);
  if (!mapping_.emplace(key, value).second) {
    throw exception(cant_insert) << "key " << key << " already exists.";
  }
  id_.invalidate();
}

void
ParameterSet::insert_or_replace_(string const& key, any const& value)
{
  check_put_local_key(key);
  mapping_[key] = value;
  id_.invalidate();
}

void
ParameterSet::insert_or_replace_compatible_(string const& key, any const& value)
{
  check_put_local_key(key);
  auto item = mapping_.find(key);
  if (item == mapping_.end()) {
    insert_(key, value);
    return;
  } else {
    if (!detail::is_nil(value)) {
      auto is_non_nil_atom = [](any const& v) {
        return !(detail::is_sequence(v) || detail::is_table(v) ||
                 detail::is_nil(v));
      };
      if (detail::is_sequence(item->second) && !detail::is_sequence(value)) {
        throw exception(cant_insert)
          << "can't use non-sequence to replace sequence.";
      } else if (detail::is_table(item->second) && !detail::is_table(value)) {
        throw exception(cant_insert) << "can't use non-table to replace table.";
      } else if (is_non_nil_atom(item->second) &&
                 (detail::is_sequence(value) || detail::is_table(value))) {
        throw exception(cant_insert)
          << "can't use non-atom to replace non-nil atom.";
      }
    }
    item->second = value;
  }
  id_.invalidate();
}

bool
ParameterSet::erase(string const& key)
{
  bool const did_erase{1u == mapping_.erase(key)};
  id_.invalidate();
  return did_erase;
}

bool
ParameterSet::key_is_type_(std::string const& key,
                           std::function<bool(std::any const&)> func) const
{
  auto split_keys = detail::get_names(key);
  auto ps = descend_(split_keys.tables());
  if (not ps) {
    throw exception(error::cant_find, key);
  }

  auto skey = detail::get_sequence_indices(split_keys.last());

  auto it = ps->mapping_.find(skey.name());
  if (it == ps->mapping_.end()) {
    throw exception(error::cant_find, key);
  }

  auto a = it->second;
  return detail::find_an_any(
           skey.indices().cbegin(), skey.indices().cend(), a) ?
           func(a) :
           throw exception(error::cant_find, key);
}

// ======================================================================
// 'put' specialization for extended_value
//
// With this specialization, the free function 'fill_src_info' is
// called, which fills an std::unordered_map whose key-value pairs
// correspond to the ParameterSet key and the location
// (filename:line#) where the key was last overridden.
//
// The main benefit of 'fill_src_info' is that it appropriately tracks
// the source information for individual sequence entries.  This is
// possible because each entry from a 'sequence_t' in the intermediate
// table is an extended_value that has a data member 'src_info'.  Note
// that whenever a printout is provided, the extended_value instances
// are no longer used, but only the mapping_ key-value pairs, which
// are the ParameterSet names and associated std::any objects.
//
// ParameterSet instances therefore do not have a natural way of
// storing source information for sequence entries because the
// extended-value information is lost.  In other words,  simply doing
//
//      srcMapping_[key] = value.src_info;
//
// whenever 'put' is called will insert the source information for the
// sequence, but not for each sequence entry.  To get around this, the
// 'fill_src_info' function appends an index number to the end of the
// sequence key (e.g. "sequence_key.1").  It is called recursively to
// allow for nested sequences.
//
// In order to access the correct source information for individual
// sequence entries in 'Prettifier::stringify()', the sequence
// index(es) must be appended to the sequence key.  Care must be taken
// to ensure that the correct ParameterSet is used to lookup the
// source information.  This is accomplished by using a stack of the
// form:
//
//     std::vector<ParameterSet const*>

namespace fhicl {
  template <>
  void
  ParameterSet::put(std::string const& key, fhicl::extended_value const& value)
  {
    auto insert = [this, &value](auto const& key) {
      using detail::encode;
      this->insert_(key, std::any(encode(value)));
      fill_src_info(value, key, srcMapping_);
    };
    detail::try_insert(insert, key);
  }
}

// ======================================================================

void
ParameterSet::walk(ParameterSetWalker& psw) const
{
  std::stack<ParameterSet const*> ps_stack;
  ps_stack.push(this);

  std::function<void(std::string const&, std::any const&)> act_on_element =
    [&psw, &ps_stack, &act_on_element](std::string const& key,
                                       std::any const& a) {
      auto const* ps = ps_stack.top();
      psw.do_before_action(key, a, ps);

      if (is_table(a)) {
        ParameterSet const* ps = &get_pset_via_any(a);
        ps_stack.push(ps);
        psw.do_enter_table(key, a);
        for (auto const& [nested_key, nested_a] : ps->mapping_) {
          act_on_element(nested_key, nested_a);
        }
        psw.do_exit_table(key, a);
        ps_stack.pop();
      } else if (is_sequence(a)) {
        psw.do_enter_sequence(key, a);
        std::size_t i{};
        for (auto const& elem : any_cast<ps_sequence_t>(a)) {
          std::string const new_key = key + "["s + std::to_string(i++) + "]";
          act_on_element(new_key, elem);
        }
        psw.do_exit_sequence(key, a);
      } else {
        psw.do_atom(key, a);
      }

      psw.do_after_action(key);
    };

  for (auto const& [key, value] : mapping_) {
    act_on_element(key, value);
  }
}

//========================================================================

string
ParameterSet::to_indented_string() const
{
  return to_indented_string(0u);
}

string
ParameterSet::to_indented_string(unsigned const initial_indent_level) const
{
  return to_indented_string(initial_indent_level, false);
}

string
ParameterSet::to_indented_string(unsigned const initial_indent_level,
                                 bool const annotate) const
{
  if (annotate) {
    return to_indented_string(initial_indent_level, print_mode::annotated);
  }
  return to_indented_string(initial_indent_level, print_mode::raw);
}

string
ParameterSet::to_indented_string(unsigned const initial_indent_level,
                                 print_mode const pm) const
{
  std::string result;
  switch (pm) {
  case print_mode::raw: {
    Prettifier p{initial_indent_level};
    walk(p);
    result = p.result();
    break;
  }
  case print_mode::annotated: {
    PrettifierAnnotated p{initial_indent_level};
    walk(p);
    result = p.result();
    break;
  }
  case print_mode::prefix_annotated: {
    PrettifierPrefixAnnotated p;
    walk(p);
    result = p.result();
    break;
  }
  }
  return result;
}

// ======================================================================
// Explicit instantiations for commonly used types

#define _INSTANTIATE_GET(FHICL_TYPE, T)                                        \
  template _DECODE_##FHICL_TYPE##_(T);                                         \
  template _GET_ONE_(T);                                                       \
  template _GET(T);                                                            \
  template _GET_WITH_DEFAULT(T);                                               \
  template _GET_IF_PRESENT(T)

_INSTANTIATE_GET(ATOM, bool);
_INSTANTIATE_GET(ATOM, int);
_INSTANTIATE_GET(ATOM, unsigned);
_INSTANTIATE_GET(ATOM, float);
_INSTANTIATE_GET(ATOM, double);
_INSTANTIATE_GET(ATOM, std::string);
_INSTANTIATE_GET(ATOM, fhicl::ParameterSet);

// Instantiate std::vector of same types except 'bool'.
_INSTANTIATE_GET(SEQUENCE, std::vector<int>);
_INSTANTIATE_GET(SEQUENCE, std::vector<unsigned>);
_INSTANTIATE_GET(SEQUENCE, std::vector<float>);
_INSTANTIATE_GET(SEQUENCE, std::vector<double>);
_INSTANTIATE_GET(SEQUENCE, std::vector<std::string>);
_INSTANTIATE_GET(SEQUENCE, std::vector<fhicl::ParameterSet>);
