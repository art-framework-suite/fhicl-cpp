#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Table.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Table<Atom<int>> t{Name{"Table"}, 1};
}
