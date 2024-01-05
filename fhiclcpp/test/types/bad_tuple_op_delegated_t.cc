#include "fhiclcpp/types/OptionalDelegatedParameter.h"
#include "fhiclcpp/types/Tuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Tuple<int, char, OptionalDelegatedParameter> t{
    Name{"Tuple"}};
}
