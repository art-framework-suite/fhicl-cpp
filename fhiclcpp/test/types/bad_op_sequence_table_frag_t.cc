#include "fhiclcpp/types/OptionalSequence.h"
#include "fhiclcpp/types/TableFragment.h"

using namespace fhicl;

int
main()
{
  // vector version
  [[maybe_unused]] OptionalSequence<TableFragment<int>> os{
    Name{"OptionalSequence"}};
  // array version
  [[maybe_unused]] OptionalSequence<TableFragment<int>, 3> os_2{
    Name{"OptionalSequence2"}};
}
