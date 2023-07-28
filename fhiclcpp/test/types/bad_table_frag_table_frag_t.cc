#include "fhiclcpp/types/TableFragment.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] TableFragment<TableFragment<int>> tf{Name{"TableFragment"}};
}
