include(CetMake)
include(CetMakeCompletions)

cet_make_exec(
  NAME fhicl-dump
  SOURCE fhicl-dump.cc
  LIBRARIES PRIVATE fhiclcpp::fhiclcpp Boost::program_options)

cet_make_exec(
  NAME fhicl-expand
  SOURCE fhicl-expand.cc
  LIBRARIES PRIVATE cetlib::cetlib Boost::program_options)

cet_make_exec(
  NAME fhicl-get
  SOURCE fhicl-get.cc
  LIBRARIES PRIVATE fhiclcpp::fhiclcpp Boost::program_options)

cet_make_exec(
  NAME fhicl-write-db
  SOURCE fhicl-write-db.cc
  LIBRARIES fhiclcpp ${SQLITE3})

cet_make_completions(fhicl-dump)
cet_make_completions(fhicl-expand)
cet_make_completions(fhicl-get)

install_headers()
install_source()
