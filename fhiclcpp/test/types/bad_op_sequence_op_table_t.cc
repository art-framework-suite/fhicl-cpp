#include "fhiclcpp/types/OptionalSequence.h"
#include "fhiclcpp/types/OptionalTable.h"

using namespace fhicl;

int main() {
  // vector version
  [[maybe_unused]] OptionalSequence<OptionalTable<int>> os{Name{"OptionalSequence"}, 1};
  // array version
  [[maybe_unused]] OptionalSequence<OptionalTable<int>, 3> os_2{Name{"OptionalSequence2"}, 1};
}
