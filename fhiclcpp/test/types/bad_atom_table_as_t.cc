#include "fhicl/types/Atom.h"
#include "fhicl/types/TableAs.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<TableAs<int>> oa{1};
}
