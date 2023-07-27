#include "fhiclcpp/types/OptionalTable.h"
#include "fhiclcpp/types/Atom.h" 

using namespace fhicl;

int main(){
  [[maybe_unused]] OptionalTable<Atom<int>> ot{Name {"OptionalTable"}, 1};
}
