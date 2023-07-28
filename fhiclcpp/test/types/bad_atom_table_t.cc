#include "fhicl/types/Atom.h"
#include "fhicl/types/Table.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<Table<int>> oa{1};
}
