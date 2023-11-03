#include "fhiclcpp/detail/ParameterSetImplHelpers.h"
#include "boost/algorithm/string.hpp"
#include "cetlib/container_algorithms.h"
#include "cetlib/split_by_regex.h"
#include "fhiclcpp/coding.h"
#include "fhiclcpp/exception.h"

#include <algorithm>
#include <regex>

namespace {
  std::regex const reBrackets{R"((\]\[|\[|\]))"};
}

namespace fhicl::detail {

  //===============================================================
  // get_names

  Keys::Keys(std::vector<std::string> const& keys, std::string const& last)
    : tables_{keys}, last_{last}
  {}
  Keys::~Keys() = default;

  std::vector<std::string> const&
  Keys::tables() const noexcept
  {
    return tables_;
  }

  std::string const&
  Keys::last() const noexcept
  {
    return last_;
  }

  Keys
  get_names(std::string const& key)
  {
    std::vector<std::string> keys;
    boost::algorithm::split(keys, key, boost::algorithm::is_any_of("."));

    // Remove empty keys
    keys.erase(std::remove(keys.begin(), keys.end(), ""), keys.end());

    if (keys.empty())
      throw fhicl::exception(fhicl::cant_find, "vacuous key");

    std::string const last = keys.back();
    keys.pop_back();

    return Keys{keys, last};
  }

  //===============================================================
  // get_sequence_indices

  SequenceKey::SequenceKey(std::string const& name,
                           std::vector<std::size_t> const& indices)
    : name_{name}, indices_{indices}
  {}

  SequenceKey::~SequenceKey() = default;

  std::string const&
  SequenceKey::name() const noexcept
  {
    return name_;
  }

  std::vector<std::size_t> const&
  SequenceKey::indices() const noexcept
  {
    return indices_;
  }

  SequenceKey
  get_sequence_indices(std::string const& key)
  {
    // Split "name[0][5][1]" according to delimiters "][", "[", and "]"
    // to give {"name","0","5","1"};
    auto tokens = cet::split_by_regex(key, reBrackets);

    auto const name = tokens.front();
    tokens.erase(tokens.begin());

    std::vector<std::size_t> indices;
    cet::transform_all(
      tokens, std::back_inserter(indices), [](std::string const& index) {
        return std::stoul(index);
      });
    return SequenceKey{name, indices};
  }

  bool
  find_an_any(std::vector<std::size_t>::const_iterator it,
              std::vector<std::size_t>::const_iterator const cend,
              std::any& a)
  {
    if (it == cend) {
      // If we got this far, that means the element must exist,
      // otherwise the previous recursive 'find_parameter' call would
      // have returned false.
      return true;
    }

    auto seq = std::any_cast<ps_sequence_t>(a);
    if (*it >= seq.size())
      return false;

    a = std::move(seq[*it]);

    return find_an_any(++it, cend, a);
  }
}
