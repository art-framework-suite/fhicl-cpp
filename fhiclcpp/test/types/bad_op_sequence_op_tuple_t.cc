#include "fhiclcpp/types/OptionalSequence.h"
#include "fhiclcpp/types/OptionalTuple.h"

using namespace fhicl;

int main() {
  // vector version
  [[maybe_unused]] OptionalSequence<OptionalTuple<int, char, double>> os{Name{"OptionalSequence"}, 1};
  // array version
  [[maybe_unused]] OptionalSequence<OptionalTuple<int, char, double>, 3> os_2{Name{"OptionalSequence2"}, 1};
}
