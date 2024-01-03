#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/TableFragment.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<TableFragment<int>> oa{1};
}
