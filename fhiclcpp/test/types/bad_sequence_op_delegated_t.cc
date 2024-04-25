#include "fhiclcpp/types/OptionalDelegatedParameter.h"
#include "fhiclcpp/types/Sequence.h"

using namespace fhicl;

int
main()
{
  // vector version
  [[maybe_unused]] Sequence<OptionalDelegatedParameter> os{Name{"Sequence"}};
  // array version
  [[maybe_unused]] Sequence<OptionalDelegatedParameter, 3> os_2{
    Name{"Sequence2"}};
}
