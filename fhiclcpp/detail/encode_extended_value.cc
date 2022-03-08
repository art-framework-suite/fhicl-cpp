// ======================================================================
//
// encode overload for extended_value
//
// ======================================================================

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/ParameterSetRegistry.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/intermediate_table.h"

using namespace fhicl;

using atom_t = intermediate_table::atom_t;
using complex_t = intermediate_table::complex_t;
using sequence_t = intermediate_table::sequence_t;
using table_t = intermediate_table::table_t;

using ps_atom_t = ParameterSet::ps_atom_t; // same as atom_t!
using ps_sequence_t = ParameterSet::ps_sequence_t;

std::any
fhicl::detail::encode(extended_value const& xval)
{
  switch (xval.tag) {
  case NIL:
  case BOOL:
  case NUMBER:
  case STRING:
    return atom_t(xval);

  case COMPLEX: {
    auto const [real, imag] = complex_t(xval);
    return '(' + real + ',' + imag + ')';
  }

  case SEQUENCE: {
    ps_sequence_t result;
    for (auto const& e : sequence_t(xval)) {
      result.push_back(encode(e));
    }
    return result;
  }

  case TABLE: {
    ParameterSet result;
    for (auto const& [key, value] : table_t(xval)) {
      if (!value.in_prolog)
        result.put(key, value);
    }
    return ParameterSetRegistry::put(result);
  }

  case TABLEID: {
    atom_t const& tableID = atom_t(xval);
    return ParameterSetID(tableID);
  }

  case UNKNOWN:
  default: {
    throw fhicl::exception(type_mismatch, "unknown extended value");
  }
  }
} // encode()
