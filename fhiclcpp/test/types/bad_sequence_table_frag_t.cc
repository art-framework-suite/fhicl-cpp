#include "fhiclcpp/types/Sequence.h"
#include "fhiclcpp/types/TableFragment.h"

using namespace fhicl;

int
main()
{
  // vector version
  [[maybe_unused]] Sequence<TableFragment<int>> os{Name{"Sequence"}};
  // array version
  [[maybe_unused]] Sequence<TableFragment<int>, 3> os_2{Name{"Sequence2"}};
}
