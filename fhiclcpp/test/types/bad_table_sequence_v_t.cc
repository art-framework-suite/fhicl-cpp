#include "fhiclcpp/types/Sequence.h"
#include "fhiclcpp/types/Table.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Table<Sequence<int>> t{Name{"Table"}, 1};
}
