#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Table.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<Table<int>> oa{1};
}
