#include "fhiclcpp/ParameterSet.h"

#include <cassert>
#include <string>
#include <vector>

using fhicl::ParameterSet;
using std::string;
using std::vector;

int
main()
{
  string const doc = "v1: [[ ],[ 1],[-1, 2,3]]";
  auto const p = ParameterSet::make(doc);
  assert(p.get_names() == vector<string>({"v1"}));
  auto vvi = p.get<vector<vector<int>>>("v1");
  assert(vvi.size() == 3);
  assert(vvi[0] == vector<int>());
  assert(vvi[1] == vector<int>({1}));
  assert(vvi[2] == vector<int>({-1, 2, 3}));
}
