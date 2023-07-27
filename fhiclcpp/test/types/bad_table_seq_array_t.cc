#include "fhiclcpp/types/Table.h"
#include <array> 

using namespace fhicl;

int main(){
  [[maybe_unused]] Table<std::array<int, 3>> t{Name {"Table"}, 1};
}
