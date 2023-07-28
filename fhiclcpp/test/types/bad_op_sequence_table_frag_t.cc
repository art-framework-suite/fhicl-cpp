#include "fhiclcpp/types/OptionalSequence.h"
#include "fhiclcpp/types/OptionalTableFragment.h"

using namespace fhicl;

int
main()
{
  // vector version
  [[maybe_unused]] OptionalSequence<OptionalTableFragment<int>> os{
    Name{"OptionalSequence"}};
  // array version
  [[maybe_unused]] OptionalSequence<OptionalTableFragment<int>, 3> os_2{
    Name{"OptionalSequence2"}};
}
