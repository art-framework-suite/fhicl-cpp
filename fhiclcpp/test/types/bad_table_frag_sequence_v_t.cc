#include "fhiclcpp/types/Sequence.h"
#include "fhiclcpp/types/TableFragment.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] TableFragment<Sequence<int>> tf{Name{"TableFragment"}};
}
