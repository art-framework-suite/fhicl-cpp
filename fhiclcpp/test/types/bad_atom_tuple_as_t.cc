#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/TupleAs.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<TupleAs<long>> oa{1};
}
