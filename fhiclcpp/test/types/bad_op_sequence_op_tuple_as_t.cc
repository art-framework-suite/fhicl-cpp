#include "fhiclcpp/types/OptionalSequence.h"
#include "fhiclcpp/types/OptionalTupleAs.h"

using namespace fhicl;

int main() {
  // vector version
  [[maybe_unused]] OptionalSequence<OptionalTupleAs<int, char, double>> os{Name{"OptionalSequence"}, 1};
  // array version
  [[maybe_unused]] OptionalSequence<OptionalTupleAs<int, char, double>, 3> os_2{Name{"OptionalSequence2"}, 1};
}
