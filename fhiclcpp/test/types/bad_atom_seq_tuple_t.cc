#include "fhiclcpp/types/Atom.h"

#include <string>
#include <tuple>

using namespace fhicl;
using namespace std::string_literals;

int main() {
  Atom<std::tuple<int, int>> tuple_atom { Name{"Tuple"s} };  
}
