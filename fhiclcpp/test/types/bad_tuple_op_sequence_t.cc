#include "fhiclcpp/types/OptionalSequence.h"
#include "fhiclcpp/types/Tuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Tuple<int, char, OptionalSequence<int>> t{Name{"Tuple"}};
}
