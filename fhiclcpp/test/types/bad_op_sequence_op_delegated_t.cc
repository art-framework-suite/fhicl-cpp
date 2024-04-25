#include "fhiclcpp/types/OptionalDelegatedParameter.h"
#include "fhiclcpp/types/OptionalSequence.h"

using namespace fhicl;

int
main()
{
  // vector version
  [[maybe_unused]] OptionalSequence<OptionalDelegatedParameter> os{
    Name{"OptionalSequence"}};
  // array version
  [[maybe_unused]] OptionalSequence<OptionalDelegatedParameter, 3> os_2{
    Name{"OptionalSequence2"}};
}
