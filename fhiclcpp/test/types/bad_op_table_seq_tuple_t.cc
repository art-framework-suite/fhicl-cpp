#include "fhiclcpp/types/OptionalTable.h"
#include <tuple> 

using namespace fhicl;

int main(){
  [[maybe_unused]] OptionalTable<tuple<int, char, double>> ot{Name {"OptionalTable"}, 1};
}
