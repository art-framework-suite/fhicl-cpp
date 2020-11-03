include(CetMake)

cet_make( ### MIGRATE-ACTION-RECOMMENDED: use cet_make_library(), build_dictionary(), basic_plugin() with explicit source lists
  LIBRARIES
  cetlib
  cetlib_except
  hep_concurrency
  ${SQLITE3}
  WITH_STATIC_LIBRARY
  SUBDIRS
  detail
  types
  types/detail
  )

install_headers(SUBDIRS detail types types/detail)
install_source(SUBDIRS detail types types/detail)
# Testing
add_subdirectory(test)
