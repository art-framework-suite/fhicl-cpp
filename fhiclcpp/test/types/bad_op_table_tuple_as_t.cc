#include "fhicl/types/OptionalTable.h"
#include "fhicl/types/TupleAs.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTable<TupleAs<long>> ot
  {
    Name{"OptionalTable", 1};
  }
