#ifndef fhiclcpp_detail_ParameterSetImplHelpers_h
#define fhiclcpp_detail_ParameterSetImplHelpers_h

#include <any>
#include <string>
#include <vector>

namespace fhicl::detail {

  //===============================================================
  // get_names

  class Keys {
  public:
    Keys(std::vector<std::string> const& keys, std::string const& last);
    ~Keys();

    std::vector<std::string> const& tables() const noexcept;
    std::string const& last() const noexcept;

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
                std::vector<std::size_t> const& indices);
    ~SequenceKey();

    std::string const& name() const noexcept;
    std::vector<std::size_t> const& indices() const noexcept;

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
