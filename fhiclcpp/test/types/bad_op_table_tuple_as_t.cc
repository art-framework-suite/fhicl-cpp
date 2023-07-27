#include "fhicl/types/TupleAs.h"
#include "fhicl/types/OptionalTable.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTable<TupleAs<long>> ot{Name{"OptionalTable", 1};
}
