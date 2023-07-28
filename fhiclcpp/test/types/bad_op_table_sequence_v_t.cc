#include "fhiclcpp/types/OptionalTable.h"
#include "fhiclcpp/types/Sequence.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTable<Sequence<int, 3>> ot{Name{"OptionalTable"}, 1};
}
