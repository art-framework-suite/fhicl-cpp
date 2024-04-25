#include "fhiclcpp/types/DelegatedParameter.h"
#include "fhiclcpp/types/Tuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Tuple<int, char, DelegatedParameter> t{Name{"Tuple"}};
}
