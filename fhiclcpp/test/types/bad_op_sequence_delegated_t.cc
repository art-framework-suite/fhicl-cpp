#include "fhiclcpp/types/OptionalSequence.h"
#include "fhiclcpp/types/DelegatedParameter.h"

using namespace fhicl;

int main() {
  // vector version
  [[maybe_unused]] OptionalSequence<DelegatedParameter> os{Name{"OptionalSequence"}, 1};
  // array version
  [[maybe_unused]] OptionalSequence<DelegatedParameter, 3> os_2{Name{"OptionalSequence2"}, 1};
}
