#include "fhiclcpp/types/OptionalTable.h"
#include "fhiclcpp/types/DelegatedParameter.h" 

using namespace fhicl;

int main(){
  [[maybe_unused]] OptionalTable<DelegatedParameter> ot{Name {"OptionalTable"}, 1};
}
