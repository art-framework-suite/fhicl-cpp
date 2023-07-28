#include "fhiclcpp/types/TableFragment.h"
#include "fhiclcpp/types/Tuple.h"
#include "fhiclcpp/types/detail/SequenceBase.h"

using namespace fhicl;

class A{};

int
main()
{
  [[maybe_unused]] Tuple<int, char, TableFragment<A>> tu{
    Name{"Tuple"}};
}
