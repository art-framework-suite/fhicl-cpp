#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/OptionalTable.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTable<Atom<int>> ot{Name{"OptionalTable"}, 1};
}
