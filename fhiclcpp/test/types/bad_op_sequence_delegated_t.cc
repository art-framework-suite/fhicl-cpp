#include "fhiclcpp/types/DelegatedParameter.h"
#include "fhiclcpp/types/OptionalSequence.h"

using namespace fhicl;

int
main()
{
  // vector version
  [[maybe_unused]] OptionalSequence<DelegatedParameter> os{
    Name{"OptionalSequence"}};
  // array version
  [[maybe_unused]] OptionalSequence<DelegatedParameter, 3> os_2{
    Name{"OptionalSequence2"}};
}
