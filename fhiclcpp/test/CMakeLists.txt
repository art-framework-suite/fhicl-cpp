include(CetMake)
include(CetTest)


cet_enable_asserts()

cet_test_env("FHICL_FILE_PATH=.")

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(hep_concurrency REQUIRED)

# Quote alternatives under different locales (used for regular expression testing)
set(open_quote "('|‘)")
set(close_quote "('|’)")

# Put everything in a different export set.
cet_register_export_set(SET_NAME Testing NAMESPACE fhiclcpp_test SET_DEFAULT)

add_subdirectory(types)

cet_test(dotted_names USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::fhiclcpp)
cet_test(hex_test LIBRARIES PRIVATE fhiclcpp::fhiclcpp)

cet_test(PSetSample INSTALL_EXAMPLE INSTALL_SOURCE
  LIBRARIES PRIVATE fhiclcpp::fhiclcpp
  DATAFILES Sample.cfg
)
cet_test(PSetTest LIBRARIES PRIVATE fhiclcpp::fhiclcpp)
cet_test(ParameterSet_t USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::fhiclcpp
  TEST_PROPERTIES
  ENVIRONMENT FHICL_FILE_PATH=${CMAKE_CURRENT_SOURCE_DIR})
cet_test(printing_helpers_t LIBRARIES PRIVATE fhiclcpp::fhiclcpp)

cet_test(get_sequence_elements_t USE_BOOST_UNIT
  LIBRARIES PRIVATE fhiclcpp::fhiclcpp
  TEST_PROPERTIES
  ENVIRONMENT FHICL_FILE_PATH=${CMAKE_CURRENT_SOURCE_DIR})

cet_test(get_via_t USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::fhiclcpp)

cet_test(equalTest USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::fhiclcpp)
cet_test(failer LIBRARIES PRIVATE fhiclcpp::fhiclcpp)
cet_make_exec(NAME fhicl-config_t NO_INSTALL LIBRARIES PRIVATE fhiclcpp::fhiclcpp)
cet_test(fhicl-expand_test.sh PREBUILT TEST_ARGS $<TARGET_FILE:fhicl-expand>)
cet_test(fhicl-expand_test_path.sh PREBUILT TEST_ARGS $<TARGET_FILE:fhicl-expand>)
cet_test(issue_0923_t.sh PREBUILT
  TEST_ARGS $<TARGET_FILE:fhicl-config_t>
  DATAFILES issue_0923_ref.txt
  DEPENDENCIES fhicl-config_t
  TEST_PROPERTIES
  ENVIRONMENT FHICL_FILE_PATH=${CMAKE_CURRENT_SOURCE_DIR})

cet_test(key_assembler_t USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::fhiclcpp)
cet_test(parse_document_test USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::fhiclcpp)
cet_test(parse_value_string_test USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::fhiclcpp)
cet_test(to_indented_string_test USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::fhiclcpp)
cet_test(to_indented_string_annotated_test LIBRARIES PRIVATE fhiclcpp::fhiclcpp
  DATAFILES
    issue-7788/to_indented_string_annotated_test.fcl
    issue-7788/to_indented_string_annotated_prolog.fcl
  REF "${CMAKE_CURRENT_SOURCE_DIR}/issue-7788/to_indented_string_annotated_test-ref.txt"
  OUTPUT_FILTERS cetlib::filter-output
)
cet_test(to_indented_string_prefix_annotated_test LIBRARIES PRIVATE fhiclcpp::fhiclcpp
  DATAFILES
    issue-7788/to_indented_string_annotated_test.fcl
    issue-7788/to_indented_string_annotated_prolog.fcl
  REF "${CMAKE_CURRENT_SOURCE_DIR}/to_indented_string_prefix_annotated_test-ref.txt"
  OUTPUT_FILTERS cetlib::filter-output
)
cet_test(to_string_test LIBRARIES PRIVATE fhiclcpp::fhiclcpp
  TEST_PROPERTIES
  ENVIRONMENT FHICL_FILE_PATH=${CMAKE_CURRENT_SOURCE_DIR})
cet_test(to_tupleTypes_test LIBRARIES PRIVATE fhiclcpp::fhiclcpp
  TEST_PROPERTIES
  ENVIRONMENT FHICL_FILE_PATH=${CMAKE_CURRENT_SOURCE_DIR})
cet_test(values_test USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::fhiclcpp)

cet_make_exec(NAME test_suite USE_BOOST_UNIT NO_INSTALL LIBRARIES PRIVATE fhiclcpp::fhiclcpp)
file(GLOB testPass RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "testFiles/pass/*_pass.fcl")
file(GLOB testFail RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "testFiles/fail/*_fail.fcl")
foreach(test_file ${testPass})
  get_filename_component(test_file_stem ${test_file} NAME_WE)
  cet_test(${test_file_stem} HANDBUILT
           TEST_EXEC test_suite
           TEST_ARGS -- ${test_file}
           TEST_PROPERTIES ENVIRONMENT FHICL_FILE_PATH=${CMAKE_CURRENT_SOURCE_DIR}
  )
endforeach()
foreach(test_file ${testFail})
  get_filename_component(test_file_stem ${test_file} NAME_WE)
  cet_test(${test_file_stem} HANDBUILT
           TEST_EXEC test_suite
           TEST_ARGS -- ${test_file}
           TEST_PROPERTIES WILL_FAIL true
           ENVIRONMENT FHICL_FILE_PATH=${CMAKE_CURRENT_SOURCE_DIR}
  )
endforeach()

cet_make_exec(NAME save-restore_t NO_INSTALL LIBRARIES PRIVATE fhiclcpp::fhiclcpp)

cet_test(save-restore-ab HANDBUILT
  TEST_EXEC save-restore_t
  TEST_ARGS ab.fcl
  DATAFILES issue-2737-fcl/ab.fcl
)

cet_test(save-restore-ba HANDBUILT
  TEST_EXEC save-restore_t
  TEST_ARGS ba.fcl
  DATAFILES issue-2737-fcl/ba.fcl
)

cet_test(intermediate_table_t USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::fhiclcpp)

cet_test(table_t_iterator_t USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::fhiclcpp)
target_compile_definitions(table_t_iterator_t PRIVATE SNIPPET_MODE=false)
cet_test(table_t_snippet_iterator_t USE_BOOST_UNIT
  SOURCE table_t_iterator_t.cc
  USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::fhiclcpp
)
target_compile_definitions(table_t_snippet_iterator_t PRIVATE SNIPPET_MODE=true)

cet_test(seq_of_seq_t LIBRARIES PRIVATE fhiclcpp::fhiclcpp)

cet_test(ParameterSetRegistry_t USE_BOOST_UNIT
  LIBRARIES PRIVATE
    fhiclcpp::fhiclcpp
    hep_concurrency::simultaneous_function_spawner
    SQLite::SQLite3
    Threads::Threads
)

cet_test(DatabaseSupport_t USE_BOOST_UNIT
  LIBRARIES PRIVATE fhiclcpp::fhiclcpp
  DATAFILES testFiles/db_0.fcl testFiles/db_1.fcl testFiles/db_2.fcl
)

cet_test(WriteSQLiteDB_t USE_BOOST_UNIT
  LIBRARIES PRIVATE fhiclcpp::fhiclcpp SQLite::SQLite3
  DATAFILES testFiles/db_2.fcl
)

get_property(WriteSQLiteDB_t_WORKDIR TEST WriteSQLiteDB_t PROPERTY WORKING_DIRECTORY)
cet_test(fhicl-write-db-test HANDBUILT
  TEST_EXEC sqlite3
  TEST_ARGS ${WriteSQLiteDB_t_WORKDIR}/test.db "select count(ID) from ParameterSets"
  TEST_PROPERTIES
  DEPENDS WriteSQLiteDB_t
  PASS_REGULAR_EXPRESSION "^3\n$"
)

cet_test(parse_shimmeddocument_test USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::fhiclcpp)

# Tests for facilities provided in the tools directory
cet_test(fhicl-get-test USE_BOOST_UNIT
  LIBRARIES PRIVATE fhiclcpp::tools
  DATAFILES fhicl-get-t.fcl)

cet_test(bad_map_shim COMPILE_ONLY LIBRARIES PRIVATE fhiclcpp::fhiclcpp
  TEST_PROPERTIES
  PASS_REGULAR_EXPRESSION
  "the expression ${open_quote}!\\(same_as<IIL, IIR>\\).*${close_quote} evaluated to ${open_quote}false${close_quote}")
