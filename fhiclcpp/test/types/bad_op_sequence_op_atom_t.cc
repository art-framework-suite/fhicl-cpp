#include "fhiclcpp/types/OptionalAtom.h"
#include "fhiclcpp/types/OptionalSequence.h"

using namespace fhicl;

int
main()
{
  // vector version
  [[maybe_unused]] OptionalSequence<OptionalAtom<int>> os{
    Name{"OptionalSequence"}};
  // array version
  [[maybe_unused]] OptionalSequence<OptionalAtom<int>, 3> os_2{
    Name{"OptionalSequence2"}};
}
