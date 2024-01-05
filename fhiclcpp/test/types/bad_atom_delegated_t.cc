#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/DelegatedParameter.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Atom<DelegatedParameter> oa{1};
}
