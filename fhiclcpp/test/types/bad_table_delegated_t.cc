#include "fhiclcpp/types/Table.h"
#include "fhiclcpp/types/DelegatedParameter.h" 

using namespace fhicl;

int main(){
  [[maybe_unused]] Table<DelegatedParameter> t{Name {"Table"}, 1};
}
