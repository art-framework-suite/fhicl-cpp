#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Sequence.h"

#include <string>

using namespace fhicl;
using namespace std::string_literals;

int
main()
{
  Atom<Sequence<int, 3>> atom_sequence{Name{"Sequence_a"s}};
}
