include(CetMake)

cet_make_library(
  LIBRARY_NAME
    fhiclcpp
  SOURCE
    coding.cc
    DatabaseSupport.cc
    detail/encode_extended_value.cc
    detail/KeyAssembler.cc
    detail/ParameterSetImplHelpers.cc
    detail/PrettifierAnnotated.cc
    detail/Prettifier.cc
    detail/PrettifierPrefixAnnotated.cc
    detail/printing_helpers.cc
    detail/ValuePrinter.cc
    exception.cc
    extended_value.cc
    intermediate_table.cc
    make_ParameterSet.cc
    ParameterSet.cc
    ParameterSetID.cc
    ParameterSetRegistry.cc
    parse.cc
    parse_shims.cc
    Protection.cc
    types/DelegatedParameter.cc
    types/detail/check_nargs_for_bounded_sequences.cc
    types/detail/MaybeDisplayParent.cc
    types/detail/NameStackRegistry.cc
    types/detail/ostream_helpers.cc
    types/detail/PrintAllowedConfiguration.cc
    types/detail/SearchAllowedConfiguration.cc
    types/detail/strip_containing_names.cc
    types/detail/ValidateThenSet.cc
    types/Name.cc
    types/OptionalDelegatedParameter.cc
  LIBRARIES PUBLIC
    cetlib::cetlib
    SQLite::SQLite3
  WITH_STATIC_LIBRARY
  )

install_headers(SUBDIRS detail types types/detail)
install_source(SUBDIRS detail types types/detail)
# Testing
add_subdirectory(test)
