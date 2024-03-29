#include "fhiclcpp/detail/printing_helpers.h"
#include "fhiclcpp/exception.h"

#include <cassert>
#include <limits>
#include <regex>

using namespace fhicl;
using namespace fhicl::detail;

//==================================================================

namespace {
  std::string const UNKNOWN_SRC{"-:1"};
  std::regex const reSequenceElement{R"(.*\[(\d+)\])"};

  inline bool
  allowed_info(std::string const& src_info)
  {
    return !src_info.empty() && src_info != UNKNOWN_SRC;
  }

  constexpr auto size_t_max = std::numeric_limits<std::size_t>::max();
}

//==================================================================

std::string
detail::print_annotated_info(std::string const& curr_info,
                             std::string const& cached_info)
{
  if (!allowed_info(curr_info))
    return "";
  std::string const tag = "  # ";
  std::string const printed_info =
    curr_info == cached_info ? "\"\"" : curr_info;
  return tag + printed_info;
}

std::string
detail::print_prefix_annotated_info(std::string const& info)
{
  std::string const printed_info = allowed_info(info) ? info : "";
  return "#SRC|" + printed_info + "|";
}

//==================================================================

std::string
detail::printed_suffix(std::string const& key, std::size_t const sz)
{
  using namespace std::string_literals;
  std::string result;

  if (sz == size_t_max)
    return result;

  std::smatch m;
  if (std::regex_match(key, m, reSequenceElement) &&
      std::stoul(m[1]) != sz - 1) {
    result = ","s;
  }
  return result;
}

//==================================================================
// table

std::string
table::printed_prefix(std::string const& key)
{
  std::string result{opening_brace()};
  if (!is_sequence_element(key))
    result = key + ": " + result;
  return result;
}

//==================================================================
// sequence

std::string
sequence::printed_prefix(std::string const& key)
{
  std::string result{opening_brace()};
  if (!is_sequence_element(key))
    result = key + ": " + result;
  return result;
}

//==================================================================
// atom

std::string
atom::printed_prefix(std::string const& key)
{
  return is_sequence_element(key) ? "" : key + ": ";
}

std::string
atom::value(std::any const& a)
{
  using ps_atom_t = std::string;
  std::string const str = std::any_cast<ps_atom_t>(a);
  return str == std::string(9, '\0') ? "@nil" : str;
}

//==================================================================
// miscellany

std::size_t
detail::index_for_sequence_element(std::string const& name)
{
  if (!is_sequence_element(name)) {
    throw exception{error::other,
                    "An error occurred while converting a name to a sequence "
                    "element index.\n"}
      << "The name '" << name << "' does not correspond to a sequence element.";
  }
  auto const b = name.find_last_of("[");
  assert(b != std::string::npos);
  auto const e = name.find_last_of("]");
  assert(e == name.length() - 1);
  auto const start = b + 1;
  assert(start < e);
  return std::stoull(name.substr(start, e - start));
}
