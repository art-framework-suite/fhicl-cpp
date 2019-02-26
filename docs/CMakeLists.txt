find_package(Sphinx REQUIRED)
 
if(NOT DEFINED SPHINX_THEME)
    set(SPHINX_THEME default)
endif()
 
if(NOT DEFINED SPHINX_THEME_DIR)
    set(SPHINX_THEME_DIR)
endif()

# configured documentation tools and intermediate build results
set(BINARY_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_build")

# Sphinx cache with pickled ReST documents
set(SPHINX_CACHE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_doctrees")

# HTML output directory
#set(SPHINX_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/html")
set(SPHINX_HTML_DIR "$ENV{MRB_TOP}/all_docs/fhiclcpp")
#message("\n\n\n\n\n\n")
#message(${SPHINX_HTML_DIR})
#message("\n\n\n\n\n\n")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/_static/ DESTINATION ${BINARY_BUILD_DIR}/_static)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/_templates/ DESTINATION ${BINARY_BUILD_DIR}/_templates)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/_extensions/ DESTINATION ${BINARY_BUILD_DIR}/_extensions)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
    "${BINARY_BUILD_DIR}/conf.py"
    @ONLY)

message(STATUS "SPHINX_EXECUTABLE: ${SPHINX_EXECUTABLE}")

add_custom_target(fhiclcpp_docs ALL
  ${SPHINX_EXECUTABLE}
  -q -b html
  -c "${BINARY_BUILD_DIR}"
  -d "${SPHINX_CACHE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${SPHINX_HTML_DIR}"
  COMMENT "Building HTML documentation with Sphinx"
  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/_extensions/redmineRoles.py)
