#include "fhiclcpp/types/OptionalSequence.h"
#include "fhiclcpp/types/OptionalDelegatedParameter.h"

using namespace fhicl;

int main() {
  // vector version
  [[maybe_unused]] OptionalSequence<OptionalDelegatedParameter> os{Name{"OptionalSequence"}, 1};
  // array version
  [[maybe_unused]] OptionalSequence<OptionalDelegatedParameter, 3> os_2{Name{"OptionalSequence2"}, 1};
}
