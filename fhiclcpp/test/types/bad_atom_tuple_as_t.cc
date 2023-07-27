#include "fhicl/types/TupleAs.h"
#include "fhicl/types/Atom.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<TupleAs<long>> oa{1};
}
