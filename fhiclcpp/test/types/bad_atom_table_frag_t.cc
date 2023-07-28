#include "fhicl/types/Atom.h"
#include "fhicl/types/TableFragment.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<TableFragment<int>> oa{1};
}
