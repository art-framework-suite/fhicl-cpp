#include "tools/fhicl_get_impl.h"

int
main(int argc, char** argv)
{
  return fhicl::detail::fhicl_get_impl(argc, argv);
}
