// ======================================================================
//
// intermediate_table
//
// ======================================================================

#include "fhiclcpp/intermediate_table.h"

#include "boost/algorithm/string.hpp"
#include "fhiclcpp/Protection.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/parse_shims_opts.h"
#include "fhiclcpp/stdmap_shims.h"

#include <cctype>
#include <cstdlib>
#include <utility>

using namespace std::string_literals;
using namespace fhicl;

using atom_t = intermediate_table::atom_t;
using complex_t = intermediate_table::complex_t;
using sequence_t = intermediate_table::sequence_t;
using table_t = intermediate_table::table_t;

using iterator = intermediate_table::iterator;
using const_iterator = intermediate_table::const_iterator;

// ----------------------------------------------------------------------
namespace {
  extended_value const&
  nil_item()
  {
    static extended_value const nil_item{false, NIL, "@nil"s};
    return nil_item;
  }

  extended_value const&
  empty_seq()
  {
    static extended_value const empty_seq{false, SEQUENCE, sequence_t{}};
    return empty_seq;
  }

  extended_value const&
  empty_tbl()
  {
    static extended_value const empty_tbl{false, TABLE, table_t{}};
    return empty_tbl;
  }
}

// ----------------------------------------------------------------------

const_iterator
intermediate_table::begin() const
{
  return std::any_cast<table_t const&>(ex_val.value).begin();
}

const_iterator
intermediate_table::end() const
{
  return std::any_cast<table_t const&>(ex_val.value).end();
}

// ----------------------------------------------------------------------

bool
intermediate_table::empty() const
{
  return std::any_cast<table_t const&>(ex_val.value).empty();
}

// ----------------------------------------------------------------------

bool
intermediate_table::insert(std::string const& key,
                           bool const in_prolog,
                           value_tag const tag,
                           std::any const& value)
{
  return insert(key, extended_value(in_prolog, tag, value));
}

bool
intermediate_table::insert(std::string const& key, extended_value const& value)
{
  auto found_val = pre_insert_(key, value);
  bool result = (found_val != nullptr);
  if (result) {
    *found_val = value;
  }
  return result;
}

bool
intermediate_table::insert(std::string const& key, extended_value&& value)
{
  auto found_val = pre_insert_(key, value);
  bool result = (found_val != nullptr);
  if (result) {
    *found_val = std::move(value);
  }
  return result;
}

// ----------------------------------------------------------------------

extended_value const&
intermediate_table::find(std::string const& key) const
{
  extended_value const* p = &ex_val;
  for (auto const& name : split(key)) {
    if (name.empty()) {
    } else if (std::isdigit(name[0])) {
      if (!p->is_a(SEQUENCE))
        throw exception(cant_find, key)
          << "-- not a sequence (at part \"" << name << "\")";
      auto const& s = std::any_cast<sequence_t const&>(p->value);
      unsigned i = std::atoi(name.c_str());
      if (s.size() <= i)
        throw exception(cant_find, key) << "(at part \"" << name << "\")";
      p = &s[i];
    } else { /* name[0] is alpha or '_' */
      if (!p->is_a(TABLE))
        throw exception(cant_find, key)
          << "-- not a table (at part \"" << name << "\")";
      auto const& t = std::any_cast<table_t const&>(p->value);
      auto it = t.find(name);
      if (it == t.end())
        throw exception(cant_find, key) << "(at part \"" << name << "\")";
      p = &it->second;
    }
  } // for
  return *p;
} // find()

// ----------------------------------------------------------------------

bool
intermediate_table::exists(std::string const& key) const
{
  extended_value const* p = &ex_val;
  for (auto const& name : split(key)) {
    if (name.empty()) {
    } else if (std::isdigit(name[0])) {
      if (!p->is_a(SEQUENCE)) {
        return false;
      }
      auto const& s = std::any_cast<sequence_t const&>(p->value);
      unsigned i = std::atoi(name.c_str());
      if (s.size() <= i) {
        return false;
      }
      p = &s[i];
    } else { /* name[0] is alpha or '_' */
      if (!p->is_a(TABLE)) {
        return false;
      }
      auto const& t = std::any_cast<table_t const&>(p->value);
      auto it = t.find(name);
      if (it == t.end()) {
        return false;
      }
      p = &it->second;
    }
  } // for
  return true;
} // exists()

void
intermediate_table::erase(std::string const& key, bool const in_prolog)
{
  // A missing part of the "a.b.c.d" chain will not cause an error; it
  // is an error for an intermediate link in the chain *not* to be a
  // table.
  auto p(&ex_val);
  auto t(std::any_cast<table_t>(&p->value));
  auto it(t->end());
  auto const names = split(key);
  if ((!in_prolog) &&
      (((it = t->find(names[0])) == t->end()) || it->second.in_prolog)) {
    return;
  }
  bool at_sequence(false);
  for (auto const& name : names) {
    if (name.empty()) {
    } else if (std::isdigit(name[0])) {
      if (!p->is_a(SEQUENCE))
        throw exception(cant_find, name)
          << "-- not a sequence (at part \"" << name << "\")";
      auto& s = std::any_cast<sequence_t&>(p->value);
      unsigned i = std::atoi(name.c_str());
      if (s.size() <= i) {
        return;
      }
      p = &s[i];
      at_sequence = true;
    } else { /* name[0] is alpha or '_' */
      if (!p->is_a(TABLE))
        throw exception(cant_find, name)
          << "-- not a table (at part \"" << name << "\")";
      at_sequence = false;
      t = std::any_cast<table_t>(&p->value);
      it = t->find(name);
      if (it == t->end()) {
        return;
      }
      p = &it->second;
      auto prot = p->protection;
      if (prot == Protection::PROTECT_ERROR) {
        throw exception(protection_violation)
          << ((name != key) ? (std::string("Part \"") + name +
                               "\" of specification to be erased\n") :
                              "")
          << '"' << name << "\" is protected on " << p->pretty_src_info()
          << '\n';
      } else if (prot == Protection::PROTECT_IGNORE) {
        return;
      }
    }
  } // for
  if (at_sequence) {
    throw fhicl::exception(unimplemented, "erase sequence member");
  }
  if (it != t->end()) {
    t->erase(it);
  }
}

extended_value*
intermediate_table::pre_insert_(std::string const& key,
                                extended_value const& value)
{
  if (!value.in_prolog) {
    auto& t = std::any_cast<table_t&>(ex_val.value);
    auto const names = split(key);
    auto it = t.find(names[0]);
    if (it != t.end() && it->second.in_prolog) {
      t.erase(it);
    }
  }
  auto located = locate_(key, value.in_prolog);
  if ((!located.first->is_a(NIL)) &&
      value.protection > located.first->protection) {
    throw exception(protection_violation)
      << '"' << key << "\" cannot be assigned with increased protection"
      << "\n(previous definition on " << located.first->pretty_src_info()
      << ")\n";
  }
  return located.second ? located.first : nullptr;
}

std::pair<extended_value*, bool>
intermediate_table::locate_(std::string const& key, bool const in_prolog)
{
  std::pair<extended_value*, bool> result(nullptr, true);
  extended_value*& p = result.first;
  p = &ex_val;
  for (auto const& name : split(key)) {
    if (name.empty()) {
    } else if (std::isdigit(name[0])) {
      if (p->is_a(NIL)) {
        *p = empty_seq();
      }
      if (!p->is_a(SEQUENCE))
        throw exception(cant_find, name)
          << "-- not a sequence (at part \"" << name << "\")";
      auto& s = std::any_cast<sequence_t&>(p->value);
      unsigned i = std::atoi(name.c_str());
      while (s.size() <= i) {
        s.push_back(nil_item());
      }
      p->set_prolog(in_prolog);
      p = &s[i];
    } else { /* name[0] is alpha or '_' */
      if (p->is_a(NIL)) {
        *p = empty_tbl();
        p->set_prolog(in_prolog);
      }
      if (!p->is_a(TABLE))
        throw exception(cant_find, name)
          << "-- not a table (at part \"" << name << "\")";
      auto& t = std::any_cast<table_t&>(p->value);
      // This will do what we need whether the key is already in the
      // map or not.
      p = &t.emplace(name, nil_item()).first->second;
      p->set_prolog(in_prolog);
    }

    auto prot = p->protection;
    if (prot == Protection::PROTECT_ERROR) {
      throw exception(protection_violation)
        << ((name != key) ? (std::string("Part \"") + name +
                             "\" of specification to be overwritten\n") :
                            "")
        << '"' << key << "\" is protected on " << p->pretty_src_info() << '\n';
    } else if (prot == Protection::PROTECT_IGNORE) {
      result.second = false;
      // Keep going since we might need definition location
      // information for specified item upstream.
    }
  } // for
  return result;
} // locate_()

std::vector<std::string>
intermediate_table::split(std::string const& key)
{
  static std::string const splitChars{shims::isSnippetMode() ? "[]" : ".[]"};
  std::vector<std::string> result;
  boost::algorithm::split(result, key, boost::algorithm::is_any_of(splitChars));
  return result;
}

// ======================================================================
