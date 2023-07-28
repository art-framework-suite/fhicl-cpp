#include "fhiclcpp/types/OptionalSequence.h"
#include "fhiclcpp/types/OptionalTuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTuple<int, char, OptionalSequence<int>> ot{Name{"OptionalTuple"}};
}
