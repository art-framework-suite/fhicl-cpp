#include "fhiclcpp/types/Table.h"
#include "fhiclcpp/types/TableFragment.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] TableFragment<Table<int>> tf{Name{"TableFragment"}};
}
