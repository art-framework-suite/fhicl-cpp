#include "fhiclcpp/type_traits.h"
#include "fhiclcpp/types/OptionalSequence.h"

using namespace fhicl;

int
main()
{
  // vector version
  [[maybe_unused]] OptionalSequence<OptionalSequence<int>> os_1{
    Name{"OptionalSequence1"}};
  [[maybe_unused]] OptionalSequence<OptionalSequence<int, 3>> os_2{
    Name{"OptionalSequence2"}};
  // array version
  [[maybe_unused]] OptionalSequence<OptionalSequence<int, 3>, 3> os_3{
    Name{"OptionalSequence3"}};
  [[maybe_unused]] OptionalSequence<OptionalSequence<int>, 3> os_4{
    Name{"OptionalSequence4"}};
}
