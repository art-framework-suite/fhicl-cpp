#include "fhiclcpp/types/OptionalTupleAs.h"
#include "fhiclcpp/types/Tuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Tuple<int, char, OptionalTupleAs<int, char, float>> t{
    Name{"Tuple"}};
}
