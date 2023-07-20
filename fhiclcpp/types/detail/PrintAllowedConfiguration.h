#ifndef fhiclcpp_types_detail_PrintAllowedConfiguration_h
#define fhiclcpp_types_detail_PrintAllowedConfiguration_h

#include "fhiclcpp/detail/Indentation.h"
#include "fhiclcpp/types/detail/MaybeDisplayParent.h"
#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/ParameterWalker.h"

#include <stack>
#include <string>
#include <unordered_set>

namespace fhicl::detail {

  class PrintAllowedConfiguration
    : public ConstParameterWalker {
  public:
    PrintAllowedConfiguration(std::ostream& os,
                              bool const showParents = false,
                              std::string const& prefix = std::string(3, ' '),
                              bool const stlf = false);
    ~PrintAllowedConfiguration();

  private:
    std::ostream& buffer_;
    Indentation indent_;
    bool suppressTopLevelFormatting_;
    std::string cachedTopLevelParameter_{};
    std::stack<MaybeDisplayParent> mps_{};
    std::unordered_set<std::string> keysWithCommas_{};
    std::unordered_set<std::string> keysWithEllipses_{};
    bool showParentsForFirstParam_;

    bool before_action(ParameterBase const&) override;
    void after_action(ParameterBase const&) override;

    void enter_table(TableBase const&) override;
    void exit_table(TableBase const&) override;

    void enter_sequence(SequenceBase const&) override;
    void exit_sequence(SequenceBase const&) override;

    void atom(AtomBase const&) override;

    void delegated_parameter(DelegateBase const&) override;

    void
    cacheTopLevelParameter(ParameterBase const& p)
    {
      if (cachedTopLevelParameter_ == "")
        cachedTopLevelParameter_ = p.key();
    }

    void
    maybeReleaseTopLevelParameter(ParameterBase const& p)
    {
      if (p.key() == cachedTopLevelParameter_)
        cachedTopLevelParameter_ = "";
    }

    bool
    suppressFormat(ParameterBase const& p)
    {
      return p.key() == cachedTopLevelParameter_;
    }
  };
}

#endif /* fhiclcpp_types_detail_PrintAllowedConfiguration_h */

// Local variables:
// mode: c++
// End:
