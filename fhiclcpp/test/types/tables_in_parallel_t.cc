#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Table.h"
#include "hep_concurrency/simultaneous_function_spawner.h"

using namespace hep::concurrency;

namespace {
  struct Config {
    fhicl::Atom<bool> flag{fhicl::Name{"Name"}};
  };
}

int
main()
{
  auto make_table = [] { fhicl::Table<Config> config{fhicl::Name{"Top"}}; };
  simultaneous_function_spawner sfs{repeated_task(10, make_table)};
}
