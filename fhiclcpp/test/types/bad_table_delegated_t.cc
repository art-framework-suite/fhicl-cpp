#include "fhiclcpp/types/DelegatedParameter.h"
#include "fhiclcpp/types/Table.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Table<DelegatedParameter> t{Name{"Table"}, 1};
}
