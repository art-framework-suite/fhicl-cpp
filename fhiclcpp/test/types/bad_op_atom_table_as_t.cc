#include "fhiclcpp/types/OptionalAtom.h"
#include "fhiclcpp/types/TableAs.h"

using namespace fhicl;

int main(){
  [[maybe_unused]] OptionalAtom<TableAs<int, long>> oa{1};
}
