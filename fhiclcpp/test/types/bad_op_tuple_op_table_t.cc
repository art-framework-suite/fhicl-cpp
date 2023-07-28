#include "fhiclcpp/types/OptionalTable.h"
#include "fhiclcpp/types/OptionalTuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTuple<int, char, OptionalTable<int>> ot{
    Name{"OptionalTuple"}};
}
