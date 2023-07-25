#ifndef fhiclcpp_types_detail_ValidateThenSet_h
#define fhiclcpp_types_detail_ValidateThenSet_h

#include "cetlib/exempt_ptr.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/ParameterWalker.h"

#include <set>
#include <string>
#include <vector>

namespace fhicl::detail {

  class ValidateThenSet : public ParameterWalker {
  public:
    ValidateThenSet(ParameterSet const& pset,
                    std::set<std::string> const& keysToIgnore);
    ~ValidateThenSet();

    void check_keys();

  private:
    bool before_action(ParameterBase& p) override;
    void after_action(ParameterBase& p) override;
    void enter_sequence(SequenceBase& p) override;
    void delegated_parameter(DelegateBase&) override;

    void enter_table(TableBase&) override;
    void atom(AtomBase&) override;

    ParameterSet const& pset_;
    std::set<std::string> ignorableKeys_;
    std::vector<std::string> userKeys_;
    std::vector<cet::exempt_ptr<ParameterBase>> missingParameters_;
  };
}

#endif /* fhiclcpp_types_detail_ValidateThenSet_h */

// Local variables:
// mode: c++
// End:
