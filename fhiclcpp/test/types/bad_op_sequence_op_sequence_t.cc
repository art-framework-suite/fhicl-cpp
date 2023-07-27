#include "fhiclcpp/types/OptionalSequence.h"

using namespace fhicl;

int main() {
  // vector version
  [[maybe_unused]] OptionalSequence<OptionalSequence<int>> os{Name{"OptionalSequence"}, 1};
  [[maybe_unused]] OptionalSequence<OptionalSequence<int, 3>> os_2{Name{"OptionalSequence"}, 1};
  // array version
  [[maybe_unused]] OptionalSequence<OptionalDelegatedParameter, 3> os_3{Name{"OptionalSequence2"}, 1};
  [[maybe_unused]] OptionalSequence<OptionalDelegatedParameter<int, 3>, 3> os_4{Name{"OptionalSequence2"}, 1};
}
