#include "fhiclcpp/types/OptionalTable.h"
#include "fhiclcpp/types/Table.h" 

using namespace fhicl;

int main(){
  [[maybe_unused]] OptionalTable<Table<int>> ot{Name {"OptionalTable"}, 1};
}
