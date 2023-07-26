#include "fhiclcpp/types/TableFragment.h"
#include "fhiclcpp/types/OptionalAtom.h"

using namespace fhicl;

int main(){
  [[maybe_unused]] OptionalAtom<TableFragment<int>> oa{1};
}
