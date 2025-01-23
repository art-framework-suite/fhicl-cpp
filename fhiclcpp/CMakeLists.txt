include(CetMakeLibrary)
include(CetRegisterExportSet)
include(InstallHeaders)
include(InstallSource)

find_package(Boost REQUIRED EXPORT)
find_package(SQLite3 REQUIRED)
find_package(TBB REQUIRED EXPORT)
find_package(cetlib REQUIRED EXPORT)
find_package(cetlib_except REQUIRED EXPORT)

cet_make_library(HEADERS_TARGET WITH_STATIC_LIBRARY
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
    parse_shims_opts.cc
    Protection.cc
  LIBRARIES
    PUBLIC
      art_plugin_support::support_macros
      cetlib::cetlib
      cetlib_except::cetlib_except
      Boost::headers
    PRIVATE
      cetlib::sqlite
      cetlib::container_algorithms
      SQLite::SQLite3
)

# Declare our secondary export set here so that it follows the default,
# upon which its targets depend.
cet_register_export_set(SET_NAME PluginSupport NAMESPACE art_plugin_support)

install_headers(SUBDIRS detail)
install_source(SUBDIRS detail)

add_subdirectory(types)

if (BUILD_TESTING)
  add_subdirectory(test)
endif()
