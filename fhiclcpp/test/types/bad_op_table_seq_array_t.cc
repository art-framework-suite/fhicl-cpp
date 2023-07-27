#include "fhiclcpp/types/OptionalTable.h"
#include <array>

using namespace fhicl;

int main(){
  [[maybe_unused]] OptionalTable<array<int, 3>> ot{1};
}
