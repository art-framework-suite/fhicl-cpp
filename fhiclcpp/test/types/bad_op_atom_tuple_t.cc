#include "fhiclcpp/types/OptionalAtom.h"
#include "fhiclcpp/types/Tuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalAtom<Tuple<int, long, double>> oa{1};
}
