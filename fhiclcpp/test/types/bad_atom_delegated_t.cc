#include "fhicl/types/DelegatedParameter.h"
#include "fhicl/types/Atom.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<DelegatedParameter> oa{1};
}
