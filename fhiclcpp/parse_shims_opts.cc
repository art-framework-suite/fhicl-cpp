#include "fhiclcpp/parse_shims_opts.h"

bool
shims::isSnippetMode(bool const m) noexcept
{
  static bool const mode{m};
  return mode;
}
