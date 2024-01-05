#include "fhiclcpp/types/TableFragment.h"
#include <array>

using namespace fhicl;

int
main()
{
  [[maybe_unused]] TableFragment<std::array<int, 3>> tf{Name{"TableFragment"}};
}
