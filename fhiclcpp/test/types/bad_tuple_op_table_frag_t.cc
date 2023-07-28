#include "fhiclcpp/types/OptionalTableFragment.h"
#include "fhiclcpp/types/Tuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Tuple<int, char, OptionalTableFragment<int>> t{
    Name{"Tuple"}};
}
