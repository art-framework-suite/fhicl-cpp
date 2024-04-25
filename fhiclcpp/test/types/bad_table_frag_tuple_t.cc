#include "fhiclcpp/types/TableFragment.h"
#include "fhiclcpp/types/Tuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] TableFragment<Tuple<int, char, double>> tf{
    Name{"TableFragment"}};
}
