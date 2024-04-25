#include "fhiclcpp/types/OptionalTable.h"
#include "fhiclcpp/types/Tuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Tuple<int, char, OptionalTable<int>> t{Name{"Tuple"}};
}
