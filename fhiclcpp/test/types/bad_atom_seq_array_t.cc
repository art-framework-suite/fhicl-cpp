#include "fhiclcpp/types/Atom.h"

#include <array>
#include <string>

using namespace fhicl;
using namespace std::string_literals;

int
main()
{
  Atom<std::array<int, 3>> array_atom{Name{"Array"s}};
}
