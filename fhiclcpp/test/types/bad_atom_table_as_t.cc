#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/TableAs.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<TableAs<int>> oa{1};
}
