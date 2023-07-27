#include "fhiclcpp/types/Table.h"

using namespace fhicl;

int main(){
  [[maybe_unused]] Table<Table<int>> t{Name {"Table"}, 1};
}
