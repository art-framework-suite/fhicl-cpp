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

foreach (test IN ITEMS
    bounded_sequences_with_defaults_t
    keymap_defaults_t
    keymap_t
    keymap_table_t
    keys_to_ignore_t
    nestedValidation_01_t
    nestedValidation_02_t
    nestedValidation_03_t
    return_types_t
    return_types_table_t
    return_value_defaults_t
    searchAllowedConfiguration_t
    table_insertionOperator_t
)
  cet_test(${test} USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::types fhiclcpp::fhiclcpp)
endforeach()

foreach (test IN ITEMS
    print_allowed_configuration_conditional_t
    print_allowed_configuration_defaults_t
    print_allowed_configuration_delegatedParameter_t
    print_allowed_configuration_optionalParameters_t
    print_allowed_configuration_t
    print_allowed_configuration_tableFragment_t
    print_tupleAs_t
)
  cet_test(${test} LIBRARIES PRIVATE fhiclcpp::types
    REF "${CMAKE_CURRENT_SOURCE_DIR}/${test}-ref.txt")
endforeach()

foreach (test IN ITEMS
    delegatedParameter_t
    optionalDelegatedParameter_t
    return_optionalValues_from_fcl_t
    return_table_values_from_fcl_t
    return_values_from_fcl_t
    tableAs_t
    tupleAs_t
)
  cet_test(${test} USE_BOOST_UNIT LIBRARIES PRIVATE fhiclcpp::types
    DATAFILES "${test}.fcl"
  )
endforeach()

cet_test(conditional_t USE_BOOST_UNIT
  LIBRARIES PRIVATE fhiclcpp::types fhiclcpp::fhiclcpp
  REF "${CMAKE_CURRENT_SOURCE_DIR}/conditional_t-ref.txt"
  DATAFILES cond_01_t.fcl cond_02_t.fcl cond_03_t.fcl
)

cet_test(validate_ParameterSet_t
  LIBRARIES PRIVATE fhiclcpp::types fhiclcpp::fhiclcpp
  REF "${CMAKE_CURRENT_SOURCE_DIR}/validate_ParameterSet_t-ref.txt"
  DATAFILES validate_ParameterSet_t.fcl
)

cet_test(validate_large_sequence_t
  LIBRARIES PRIVATE fhiclcpp::types)

cet_test(tables_in_parallel_t LIBRARIES PRIVATE
  fhiclcpp::types hep_concurrency::simultaneous_function_spawner)

set(test_exp_table_frag
  "note: the expression ‘!\\(\\(is_stl_sequence<T>\\) || \\(is_fhicl_type<T>\\) || \\(is_fhicl_table_fragment<T>\\) || \\(is_delegated_param<T>\\)\\).*evaluated to ‘false’"
)
set(test_exp_atom_table
  "note: the expression ‘!\\(\\(is_sequence_type_param<T>\\) || \\(is_fhicl_type_param<T>\\) || \\(is_table_fragment_param<T>\\) || \\(is_delegated_param<T>\\)\\).*evaluated to 'false'"
)
foreach (bad_test IN ITEMS
    atom delegated non_class seq_array seq_tuple seq_vector sequence_a sequence_v table table_as table_frag tuple tuple_as
  )
  cet_test(bad_table_frag_${bad_test}_t COMPILE_ONLY LIBRARIES PRIVATE fhiclcpp::types
    TEST_PROPERTIES PASS_REGULAR_EXPRESSION "${test_exp_table_frag}"
  )
  if (bad_test STREQUAL non_class)
    continue()
  endif()
  cet_test(bad_atom_${bad_test}_t COMPILE_ONLY LIBRARIES PRIVATE fhiclcpp::types
    TEST_PROPERTIES PASS_REGULAR_EXPRESSION "${test_exp_atom_table}"
  )
  cet_test(bad_op_atom_${bad_test}_t COMPILE_ONLY LIBRARIES PRIVATE fhiclcpp::types
    TEST_PROPERTIES PASS_REGULAR_EXPRESSION "${test_exp_atom_table}"
  )
  cet_test(bad_op_table_${bad_test}_t COMPILE_ONLY LIBRARIES PRIVATE fhiclcpp::types
    TEST_PROPERTIES PASS_REGULAR_EXPRESSION "${test_exp_atom_table}"
  )
  cet_test(bad_table_${bad_test}_t COMPILE_ONLY LIBRARIES PRIVATE fhiclcpp::types
    TEST_PROPERTIES PASS_REGULAR_EXPRESSION "${test_exp_atom_table}"
  )
endforeach()
set(test_exp_sequence
  "note: the expression ‘!\\(\\(is_optional_param<T>\\) || \\(is_delegated_param<T>\\) || \\(is_fhicl_table_fragment<T>\\)\\).*evaluated to ‘false’"
)
set(test_exp_tuple
  "note: the expression ‘\\(\\.\\.\\. && tuple_compatible<ARGS>\\).*evaluated to ‘false’"
)
foreach (bad_test IN ITEMS
    delegated op_atom op_delegated op_sequence op_table op_table_as op_tuple op_tuple_as table_frag
  )
  cet_test(bad_op_sequence_${bad_test}_t COMPILE_ONLY LIBRARIES PRIVATE fhiclcpp::types
    TEST_PROPERTIES PASS_REGULAR_EXPRESSION "${test_exp_sequence}"
  )
  cet_test(bad_sequence_${bad_test}_t COMPILE_ONLY LIBRARIES PRIVATE fhiclcpp::types
    TEST_PROPERTIES PASS_REGULAR_EXPRESSION "${test_exp_sequence}"
  )
  cet_test(bad_op_tuple_${bad_test}_t COMPILE_ONLY LIBRARIES PRIVATE fhiclcpp::types
    TEST_PROPERTIES PASS_REGULAR_EXPRESSION "${test_exp_tuple}"
  )
  cet_test(bad_tuple_${bad_test}_t COMPILE_ONLY LIBRARIES PRIVATE fhiclcpp::types
    TEST_PROPERTIES PASS_REGULAR_EXPRESSION "${test_exp_tuple}"
  )
endforeach()
