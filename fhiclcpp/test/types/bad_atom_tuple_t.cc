#include "fhicl/types/Tuple.h"
#include "fhicl/types/Atom.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<Tuple<int, char, double>> oa{1};
}
