#include "fhiclcpp/types/Atom.h"

#include <string>
#include <vector>

using namespace fhicl;
using namespace std::string_literals;

int
main()
{
  Atom<std::vector<int>> vector_atom{Name{"Vector"s}};
}
