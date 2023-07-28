#include "fhiclcpp/types/OptionalTupleAs.h"
#include "fhiclcpp/types/Tuple.h"

#include <tuple>

using namespace fhicl;

  struct ThreeNumbers {
    int i_, j_, k_;
    ThreeNumbers(int i, int j, int k) : i_{i}, j_{j}, k_{k} {}
  };

int
main()
{
  [[maybe_unused]] Tuple<int, char, OptionalTupleAs<ThreeNumbers(int, int, int)>> t{
    Name{"Tuple"}};
}
