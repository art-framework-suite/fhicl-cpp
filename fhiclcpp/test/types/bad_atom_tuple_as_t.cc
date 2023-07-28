#include "fhicl/types/Atom.h"
#include "fhicl/types/TupleAs.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<TupleAs<long>> oa{1};
}
