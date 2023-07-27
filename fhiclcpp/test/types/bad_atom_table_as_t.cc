#include "fhicl/types/TableAs.h"
#include "fhicl/types/Atom.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<TableAs<int>> oa{1};
}
