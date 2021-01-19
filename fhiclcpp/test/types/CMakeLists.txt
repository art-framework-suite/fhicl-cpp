# ======================================================================
#
# Testing for fhiclcpp-types: Atom<>
#                             Sequence<>
#                             Table<>
#                             TableFragment<>
#                             Tuple<>
#
# The below tests examine the following scenarios where in what
# follows, ’T’ represents a type supported by an Atom<> and ’S’
# represents an explicitly constructed struct that may contain
# Atom<>, Sequence<>, Table(Fragment)<>, or Tuple<> objects.
#
#    [ 1] Atom<T>;
#    [ 2] Sequence<T>
#    [ 3] Sequence<T,SZ>
#    [ 4] Tuple<T...>
#    [ 5] Tuple< Sequence<T>, U...>
#    [ 6] Tuple< Sequence<T,SZ>, U...>
#    [ 7] Tuple< Tuple<T...>,U...>
#    [ 8] Sequence< Tuple<T...> >
#    [ 9] Sequence< Tuple<T...>, SZ >
#    [10] Sequence< Sequence<T> >
#    [11] Sequence< Sequence<T,SZ> >
#    [12] Sequence< Sequence<T>, SZ >
#    [13] Sequence< Sequence<T,SZ>, SZ >
#
#    14-21 cannot support default arguments (since Table<> cannot have a default)
#
#    [14] Table<S>
#    [15] Sequence< Table<S> >
#    [16] Sequence< Table<S>, SZ >
#    [17] Tuple< Table<S>, U... >
#    [18] Tuple< Sequence< Table<S> >, U... >
#    [19] Tuple< Sequence< Table<S>, SZ>, U... >
#    [20] Sequence< Tuple< Table<S>, U... > >
#    [21] Sequence< Tuple< Table<S>, U... >, SZ>
#
#    The TableFragment<> does not support a default
#
#    [22] TableFragment<S>
#
# ======================================================================

set(TESTS_REQUIRING_BOOST_UNIT_AND_FHICLCPP
    keymap_t
    keymap_defaults_t
    keymap_table_t
    keys_to_ignore_t
    return_types_t
    return_types_table_t
    return_value_defaults_t
    searchAllowedConfiguration_t
    table_insertionOperator_t
    bounded_sequences_with_defaults_t
    nestedValidation_01_t
    nestedValidation_02_t
    nestedValidation_03_t)
foreach(test IN LISTS TESTS_REQUIRING_BOOST_UNIT_AND_FHICLCPP)
  cet_test(${test} USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::fhiclcpp)
endforeach()

set(TESTS_WITH_REF_FILES
    print_tupleAs_t
    print_allowed_configuration_t
    print_allowed_configuration_optionalParameters_t
    print_allowed_configuration_tableFragment_t
    print_allowed_configuration_delegatedParameter_t
    print_allowed_configuration_conditional_t
    print_allowed_configuration_defaults_t)
foreach(test IN LISTS TESTS_WITH_REF_FILES)
  cet_test(${test}
           REF "${CMAKE_CURRENT_SOURCE_DIR}/${test}-ref.txt"
           LIBRARIES PRIVATE fhiclcpp::fhiclcpp)
endforeach()

set(TESTS_WITH_DATAFILES
    tableAs_t
    tupleAs_t
    delegatedParameter_t
    optionalDelegatedParameter_t
    return_values_from_fcl_t
    return_optionalValues_from_fcl_t
    return_table_values_from_fcl_t)
foreach(test IN LISTS TESTS_WITH_DATAFILES)
  cet_test(${test} USE_BOOST_UNIT
           DATAFILES ${test}.fcl
           LIBRARIES PRIVATE fhiclcpp::fhiclcpp)
endforeach()

cet_test(conditional_t USE_BOOST_UNIT
  DATAFILES
    cond_01_t.fcl
    cond_02_t.fcl
    cond_03_t.fcl
  REF "${CMAKE_CURRENT_SOURCE_DIR}/conditional_t-ref.txt"
  LIBRARIES PRIVATE fhiclcpp::fhiclcpp)

cet_test(validate_ParameterSet_t
  DATAFILES validate_ParameterSet_t.fcl
  REF "${CMAKE_CURRENT_SOURCE_DIR}/validate_ParameterSet_t-ref.txt"
  LIBRARIES PRIVATE fhiclcpp::fhiclcpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
cet_test(tables_in_parallel_t
  LIBRARIES PRIVATE fhiclcpp::fhiclcpp Threads::Threads)
