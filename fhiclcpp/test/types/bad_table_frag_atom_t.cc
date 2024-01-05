#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/TableFragment.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Table<Atom<int>> tf{Name{"TableFragment"}};
}
