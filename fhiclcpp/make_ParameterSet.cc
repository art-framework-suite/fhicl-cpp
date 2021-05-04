// ======================================================================
//
// make_ParameterSet - intermediate_table -> ParameterSet
//
// ======================================================================

#include "fhiclcpp/make_ParameterSet.h"

#include "fhiclcpp/ParameterSet.h"

using namespace fhicl;

// ----------------------------------------------------------------------

void
fhicl::make_ParameterSet(intermediate_table const& tbl, ParameterSet& ps)
{
  ps = ParameterSet::make(tbl);
}

// ----------------------------------------------------------------------

void
fhicl::make_ParameterSet(extended_value const& xval, ParameterSet& ps)
{
  ps = ParameterSet::make(xval);
}

// ----------------------------------------------------------------------

void
fhicl::make_ParameterSet(std::string const& str, ParameterSet& ps)
{
  ps = ParameterSet::make(str);
}

// ----------------------------------------------------------------------

void
fhicl::make_ParameterSet(std::string const& filename,
                         cet::filepath_maker& maker,
                         ParameterSet& ps)
{
  ps = ParameterSet::make(filename, maker);
}

// ======================================================================
