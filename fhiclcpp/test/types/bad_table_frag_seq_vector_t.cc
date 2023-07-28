#include "fhiclcpp/types/TableFragment.h"
#include <vector>

using namespace fhicl;

int
main()
{
  [[maybe_unused]] TableFragment<std::vector<int>> tf{Name{"TableFragment"}};
}
