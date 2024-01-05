#include "fhiclcpp/types/TableAs.h"
#include "fhiclcpp/types/TableFragment.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] TableFragment<TableAs<int>> tf{Name{"TableFragment"}};
}
