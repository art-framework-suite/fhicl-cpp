#include "fhiclcpp/types/TableFragment.h"
#include "fhiclcpp/types/TableAs.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] TableFragment<TableAs<int>> tf{Name{"TableFragment"}};
}
