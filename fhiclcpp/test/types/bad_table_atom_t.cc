#include "fhiclcpp/types/Table.h"
#include "fhiclcpp/types/Atom.h" 

using namespace fhicl;

int main(){
  [[maybe_unused]] Table<Atom<int>> t{Name {"Table"}, 1};
}
