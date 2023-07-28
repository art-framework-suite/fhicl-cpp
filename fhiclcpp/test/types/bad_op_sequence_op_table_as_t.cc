#include "fhiclcpp/types/OptionalSequence.h"
#include "fhiclcpp/types/OptionalTableAs.h"

using namespace fhicl;

int
main()
{
  // vector version
  [[maybe_unused]] OptionalSequence<OptionalTableAs<int>> os{
    Name{"OptionalSequence"}};
  // array version
  [[maybe_unused]] OptionalSequence<OptionalTableAs<int>, 3> os_2{
    Name{"OptionalSequence2"}};
}
