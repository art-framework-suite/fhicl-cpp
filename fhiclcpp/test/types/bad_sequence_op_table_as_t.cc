#include "fhiclcpp/types/OptionalTableAs.h"
#include "fhiclcpp/types/Sequence.h"

using namespace fhicl;

int
main()
{
  // vector version
  [[maybe_unused]] Sequence<OptionalTableAs<int>> os{Name{"Sequence"}};
  // array version
  [[maybe_unused]] Sequence<OptionalTableAs<int>, 3> os_2{Name{"Sequence2"}};
}
