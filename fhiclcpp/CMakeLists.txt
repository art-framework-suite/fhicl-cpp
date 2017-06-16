include(CetMake)

cet_make(
  LIBRARIES
  ${CETLIB}
  ${CETLIB_EXCEPT}
  ${SQLITE3}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_REGEX_LIBRARY}
  WITH_STATIC_LIBRARY
  SUBDIRS
  detail
  types
  types/detail
  )

install_headers(SUBDIRS detail types types/detail)
install_source(SUBDIRS detail types types/detail)
# Testing
#add_subdirectory(test)
