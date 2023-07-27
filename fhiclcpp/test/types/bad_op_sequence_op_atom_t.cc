#include "fhiclcpp/types/OptionalSequence.h"
#include "fhiclcpp/types/OptionalAtom.h"

using namespace fhicl;

int main() {
  // vector version
  [[maybe_unused]] OptionalSequence<OptionalAtom<int>> os{Name{"OptionalSequence"}, 1};
  // array version
  [[maybe_unused]] OptionalSequence<OptionalAtom<int>, 3> os_2{Name{"OptionalSequence2"}, 1};
}
