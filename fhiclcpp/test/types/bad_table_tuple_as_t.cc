#include "fhiclcpp/types/Table.h"
#include "fhiclcpp/types/TupleAs.h" 

using namespace fhicl;

int main(){
  [[maybe_unused]] Table<TupleAs<int, char, double>> t{Name {"Table"}, 1};
}
