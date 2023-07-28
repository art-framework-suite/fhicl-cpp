#include "fhiclcpp/types/DelegatedParameter.h"
#include "fhiclcpp/types/OptionalTuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTuple<int, char, DelegatedParameter> ot{Name{"OptionalTuple"}};
}
