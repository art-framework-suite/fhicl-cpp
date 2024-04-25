#include "fhiclcpp/types/TableFragment.h"
#include <tuple>

using namespace fhicl;

int
main()
{
  [[maybe_unused]] TableFragment<std::tuple<int, char, float>> tf{
    Name{"TableFragment"}};
}
