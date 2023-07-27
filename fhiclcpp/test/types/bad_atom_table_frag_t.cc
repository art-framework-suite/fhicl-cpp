#include "fhicl/types/TableFragment.h"
#include "fhicl/types/Atom.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<TableFragment<int>> oa{1};
}
