#include "fhiclcpp/types/DelegatedParameter.h"
#include "fhiclcpp/types/Sequence.h"

using namespace fhicl;

int
main()
{
  // vector version
  [[maybe_unused]] Sequence<DelegatedParameter> os{Name{"Sequence"}};
  // array version
  [[maybe_unused]] Sequence<DelegatedParameter, 3> os_2{Name{"Sequence2"}};
}
