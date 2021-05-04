#ifndef fhiclcpp_parse_h
#define fhiclcpp_parse_h

// ======================================================================
//
// parse
//
// ======================================================================

#include "cetlib/filepath_maker.h"
#include "fhiclcpp/fwd.h"

#include <istream>
#include <string>

namespace fhicl {

  bool parse_value_string(std::string const& s,
                          extended_value& v,
                          std::string& unparsed);

  intermediate_table parse_document(std::string const& filename,
                                    cet::filepath_maker& maker);

  intermediate_table parse_document(std::istream& is,
                                    cet::filepath_maker& maker);

  intermediate_table parse_document(std::string const& s);

} // namespace fhicl

// ======================================================================

#endif /* fhiclcpp_parse_h */

// Local Variables:
// mode: c++
// End:
