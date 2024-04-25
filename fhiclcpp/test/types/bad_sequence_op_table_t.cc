#include "fhiclcpp/types/OptionalTable.h"
#include "fhiclcpp/types/Sequence.h"

using namespace fhicl;

int
main()
{
  // vector version
  [[maybe_unused]] Sequence<OptionalTable<int>> os{Name{"Sequence"}};
  // array version
  [[maybe_unused]] Sequence<OptionalTable<int>, 3> os_2{Name{"Sequence2"}};
}
