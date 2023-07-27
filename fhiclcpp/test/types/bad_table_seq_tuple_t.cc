#include "fhiclcpp/types/Table.h"
#include <tuple> 

using namespace fhicl;

int main(){
  [[maybe_unused]] Table<std::tuple<int, char, float>> t{Name {"Table"}, 1};
}
