include(CetMake)
include(CetMakeCompletions)

cet_make_library(SOURCE fhicl_get_impl.cc
  LIBRARIES PRIVATE
    fhiclcpp::fhiclcpp
    cetlib::parsed_program_options
    cetlib_except::cetlib_except
    Boost::program_options
)

cet_make_exec(NAME fhicl-dump
  LIBRARIES PRIVATE
    fhiclcpp::fhiclcpp
    cetlib::parsed_program_options
    cetlib::cetlib
    cetlib_except::cetlib_except
    Boost::program_options
)

cet_make_exec(NAME fhicl-expand
  LIBRARIES PRIVATE
    cetlib::parsed_program_options
    cetlib::cetlib
    cetlib::container_algorithms
    Boost::program_options
)

cet_make_exec(NAME fhicl-get
  LIBRARIES PRIVATE fhiclcpp::tools
)

cet_make_exec(NAME fhicl-write-db
  LIBRARIES PRIVATE fhiclcpp::fhiclcpp SQLite::SQLite3)

cet_make_completions(fhicl-dump)
cet_make_completions(fhicl-expand)
cet_make_completions(fhicl-get)

install_headers()
install_source()
