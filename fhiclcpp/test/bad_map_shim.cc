#include "fhiclcpp/stdmap_shims.h"
#include "fhiclcpp/parse_shims_opts.h"

#include <string>

int main(){
  shims::map<std::string, double> m;
  m.emplace("twopointfive", 2.5);
  auto iir = m.find("twopointfive"); 
  auto iil = m.find("twopointfive");
  bool b = m == m;
}
