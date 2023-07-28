#include "fhiclcpp/types/Table.h"
#include "fhiclcpp/types/Tuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Table<Tuple<int, char, double>> t{Name{"Table"}};
}
