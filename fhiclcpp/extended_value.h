#ifndef fhiclcpp_extended_value_h
#define fhiclcpp_extended_value_h

// ======================================================================
//
// extended_value
//
// ======================================================================

#include "fhiclcpp/Protection.h"
#include "fhiclcpp/fwd.h"
#include "fhiclcpp/stdmap_shims.h"

#include <any>
#include <string>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------

namespace fhicl {
  enum value_tag {
    UNKNOWN,
    NIL,
    BOOL,
    NUMBER,
    COMPLEX,
    STRING,
    SEQUENCE,
    TABLE,
    TABLEID
  };
}

// ----------------------------------------------------------------------

class fhicl::extended_value {
public:
  using atom_t = std::string;
  using complex_t = std::pair<std::string, std::string>;
  using sequence_t = std::vector<extended_value>;
  using table_t = shims::map<std::string, extended_value>;

  extended_value();
  ~extended_value();

  extended_value(bool const in_prolog,
                 value_tag const tag,
                 std::any const value,
                 Protection const protection,
                 std::string src = {})
    : in_prolog{in_prolog}
    , tag{tag}
    , value{value}
    , src_info{std::move(src)}
    , protection{protection}
  {}

  extended_value(bool const in_prolog,
                 value_tag const tag,
                 std::any const value,
                 std::string src = {})
    : in_prolog{in_prolog}, tag{tag}, value{value}, src_info{std::move(src)}
  {}

  bool
  is_a(value_tag const t) const noexcept
  {
    return t == tag;
  }

  std::string to_string() const;

  void set_prolog(bool new_prolog_state);

  void
  set_src_info(std::string const& src)
  {
    src_info = src;
  }

  void
  reset_protection()
  {
    // See notes below.
    protection = Protection::NONE;
  }

  std::string pretty_src_info() const;

  operator atom_t() const { return std::any_cast<atom_t>(value); }
  operator complex_t() const { return std::any_cast<complex_t>(value); }
  operator sequence_t() const { return std::any_cast<sequence_t>(value); }
  operator table_t() const { return std::any_cast<table_t>(value); }

  bool in_prolog{false};
  value_tag tag{UNKNOWN};
  std::any value{};
  std::string src_info{};

  // Protection corresponds to the binding of a name to a value, and
  // not the value per se.  The protection data member is thus
  // separate from the other data of this class but retained here for
  // navigational convenience.  Care must therefore be taken when
  // implementing '@local::' (e.g.):
  //
  //   x @protect_ignore: 14  // -> protection for x is PROTECT_IGNORE
  //   y: @local::x           // -> protection for y should be NONE
  //
  // The reset_protection() member function is called while parsing y
  // to ensure that x's protection does not propagate to y.
  Protection protection{Protection::NONE};

}; // extended_value

// ======================================================================

#endif /* fhiclcpp_extended_value_h */

// Local Variables:
// mode: c++
// End:
