#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Tuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<Tuple<int, char, double>> oa{1};
}
