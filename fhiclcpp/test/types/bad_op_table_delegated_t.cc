#include "fhiclcpp/types/DelegatedParameter.h"
#include "fhiclcpp/types/OptionalTable.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTable<DelegatedParameter> ot{Name{"OptionalTable"},
                                                        1};
}
