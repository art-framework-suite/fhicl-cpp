#include "fhiclcpp/types/OptionalTuple.h"
#include "fhiclcpp/types/TableFragment.h"

using namespace fhicl;

class A {};

int
main()
{
  [[maybe_unused]] OptionalTuple<int, char, TableFragment<A>> ot{
    Name{"OptionalTuple"}};
}
