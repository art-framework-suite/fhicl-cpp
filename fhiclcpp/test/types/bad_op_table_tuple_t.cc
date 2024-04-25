#include "fhiclcpp/types/OptionalTable.h"
#include "fhiclcpp/types/Tuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTable<Tuple<int, double, long>> ot{
    Name{"OptionalTable"}, 1};
}
