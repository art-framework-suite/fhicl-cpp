cet_make_library(LIBRARY_NAME fhiclcpp_types
  SOURCE
    DelegatedParameter.cc
    Name.cc
    OptionalDelegatedParameter.cc
    detail/MaybeDisplayParent.cc
    detail/NameStackRegistry.cc
    detail/ParameterBase.cc
    detail/ParameterWalkerImpl.cc
    detail/PrintAllowedConfiguration.cc
    detail/SearchAllowedConfiguration.cc
    detail/TableBase.cc
    detail/TableMemberRegistry.cc
    detail/ValidateThenSet.cc
    detail/check_nargs_for_bounded_sequences.cc
    detail/ostream_helpers.cc
    detail/strip_containing_names.cc
  LIBRARIES
    PUBLIC
      fhiclcpp::fhiclcpp
      cetlib::container_algorithms
      cetlib::cetlib
      TBB::tbb
)

cet_make_library(LIBRARY_NAME plugin_config_macros INTERFACE
  EXPORT_SET PluginSupport
  SOURCE AllowedConfigurationMacro.h
  LIBRARIES INTERFACE
    fhiclcpp::types
    cetlib::compiler_macros
)

target_link_libraries(plugin_config_macros INTERFACE $<TARGET_NAME:fhiclcpp_types>)

install_headers(SUBDIRS detail)
install_source(SUBDIRS detail)
