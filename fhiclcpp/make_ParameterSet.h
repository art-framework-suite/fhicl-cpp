#ifndef fhiclcpp_make_ParameterSet_h
#define fhiclcpp_make_ParameterSet_h

// ======================================================================
//
// make_ParameterSet - alternate representations -> ParameterSet
//
// ======================================================================

#include "cetlib/filepath_maker.h"
#include "fhiclcpp/fwd.h"

#include <string>

// ----------------------------------------------------------------------

namespace fhicl {

  [[deprecated("Please use the ParameterSet::make(intermediate_table const&) "
               "function instead.")]] void
  make_ParameterSet(intermediate_table const& tbl, ParameterSet& ps);

  [[deprecated("Please use the ParameterSet::make(extended_value const&) "
               "function instead.")]] void
  make_ParameterSet(extended_value const& xval, ParameterSet& ps);

  [[deprecated("Please use the ParameterSet::make(std::string const&) "
               "function instead.")]] void
  make_ParameterSet(std::string const& str, ParameterSet& ps);

  [[deprecated("Please use the ParameterSet::make(std::string const& filename, "
               "cet::filepath_maker&) function instead.")]] void
  make_ParameterSet(std::string const& filename,
                    cet::filepath_maker& maker,
                    ParameterSet& ps);

} // fhicl

// ======================================================================

#endif /* fhiclcpp_make_ParameterSet_h */

// Local Variables:
// mode: c++
// End:
