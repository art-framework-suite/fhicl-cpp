#include "fhicl/types/Atom.h"
#include "fhicl/types/DelegatedParameter.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<DelegatedParameter> oa{1};
}
