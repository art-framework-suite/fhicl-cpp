#include "fhicl/types/DelegatedParameter.h"
#include "fhicl/types/OptionalAtom.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalAtom<DelegatedParameter> oa{1};
}
