#include "fhiclcpp/types/OptionalSequence.h"
#include "fhiclcpp/types/Sequence.h"

using namespace fhicl;

int
main()
{
  // vector version
  [[maybe_unused]] Sequence<OptionalSequence<int>> os_1{Name{"Sequence1"}};
  [[maybe_unused]] Sequence<OptionalSequence<int, 3>> os_2{Name{"Sequence2"}};
  // array version
  [[maybe_unused]] Sequence<OptionalSequence<int, 3>, 3> os_3{
    Name{"Sequence3"}};
  [[maybe_unused]] Sequence<OptionalSequence<int>, 3> os_4{Name{"Sequence4"}};
}
