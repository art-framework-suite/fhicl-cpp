#include "fhiclcpp/types/TableFragment.h"
#include "fhiclcpp/types/TupleAs.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] TableFragment<TupleAs<int, char, double>> tf{
    Name{"TableFragment"}};
}
