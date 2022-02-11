#ifndef fhiclcpp_detail_ParameterSetImplHelpers_h
#define fhiclcpp_detail_ParameterSetImplHelpers_h

#include "fhiclcpp/coding.h"

#include <any>
#include <string>
#include <vector>

namespace fhicl::detail {

  //===============================================================
  // get_names

  class Keys {
  public:
    Keys(std::vector<std::string> const& keys, std::string const& last)
      : tables_{keys}, last_{last}
    {}

    auto const&
    tables() const noexcept
    {
      return tables_;
    }
    auto const&
    last() const noexcept
    {
      return last_;
    }

  private:
    std::vector<std::string> tables_;
    std::string last_;
  };

  Keys get_names(std::string const& key);

  //===============================================================
  // get_sequence_indices

  class SequenceKey {
  public:
    SequenceKey(std::string const& name,
                std::vector<std::size_t> const& indices)
      : name_{name}, indices_{indices}
    {}

    auto const&
    name() const noexcept
    {
      return name_;
    }
    auto const&
    indices() const noexcept
    {
      return indices_;
    }

  private:
    std::string name_;
    std::vector<std::size_t> indices_;
  };

  SequenceKey get_sequence_indices(std::string const& key);

  //===============================================================
  // find_an_any

  bool find_an_any(std::vector<std::size_t>::const_iterator it,
                   std::vector<std::size_t>::const_iterator const cend,
                   std::any& a);
}

#endif /* fhiclcpp_detail_ParameterSetImplHelpers_h */

// Local variables:
// mode: c++
// End:
