#include "fhicl/types/Table.h"
#include "fhicl/types/Atom.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<Table<int>> oa{1};
}
