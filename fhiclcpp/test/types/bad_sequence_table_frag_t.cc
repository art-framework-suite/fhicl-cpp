#include "fhiclcpp/types/OptionalTableFragment.h"
#include "fhiclcpp/types/Sequence.h"

using namespace fhicl;

int
main()
{
  // vector version
  [[maybe_unused]] Sequence<OptionalTableFragment<int>> os{Name{"Sequence"}};
  // array version
  [[maybe_unused]] Sequence<OptionalTableFragment<int>, 3> os_2{
    Name{"Sequence2"}};
}
