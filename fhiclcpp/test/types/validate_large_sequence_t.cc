#include "fhiclcpp/ParameterSet.h"

#include "fhiclcpp/types/Sequence.h"
#include "fhiclcpp/types/Table.h"

#include <cassert>
#include <iostream>
#include <numeric>

using namespace fhicl;

namespace {
  struct Configuration {
    Sequence<int> numbers{Name("many_numbers")};
  };
}

int
main()
{
  ParameterSet pset;
  std::vector<int> numbers(5000);
  std::iota(begin(numbers), end(numbers), 0);
  pset.put("many_numbers", numbers);
  auto const numbers_str = pset.to_indented_string();

  auto new_pset = ParameterSet::make(numbers_str);
  auto const a = new_pset.get<std::vector<int>>("many_numbers");
  assert(numbers == a);

  Table<Configuration> validated_config{new_pset};
  auto const b = validated_config().numbers();
  assert(numbers == b);
}
