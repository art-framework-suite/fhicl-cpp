#include "fhiclcpp/types/Atom.h"

#include <string>

using namespace fhicl;
using namespace std::string_literals;

int main() {
  Atom<Atom<int>> atom_atom { Name{"Atom"s} };  
}
