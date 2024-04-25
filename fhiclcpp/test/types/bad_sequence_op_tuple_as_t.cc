#include "fhiclcpp/types/OptionalTupleAs.h"
#include "fhiclcpp/types/Sequence.h"

using namespace fhicl;

int
main()
{
  // vector version
  [[maybe_unused]] Sequence<OptionalTupleAs<int, char, double>> os{
    Name{"Sequence"}};
  // array version
  [[maybe_unused]] Sequence<OptionalTupleAs<int, char, double>, 3> os_2{
    Name{"Sequence2"}};
}
