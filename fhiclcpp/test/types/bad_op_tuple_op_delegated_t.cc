#include "fhiclcpp/types/OptionalDelegatedParameter.h"
#include "fhiclcpp/types/OptionalTuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTuple<int, char, OptionalDelegatedParameter> ot{
    Name{"OptionalTuple"}};
}
