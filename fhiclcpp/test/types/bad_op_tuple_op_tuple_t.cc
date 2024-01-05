#include "fhiclcpp/types/OptionalTuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTuple<int, char, OptionalTuple<int, char, float>> ot{
    Name{"OptionalTuple"}};
}
