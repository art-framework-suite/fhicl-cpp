#include "fhiclcpp/types/OptionalTuple.h"
#include "fhiclcpp/types/Tuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Tuple<int, char, OptionalTuple<int, char, float>> t{
    Name{"Tuple"}};
}
