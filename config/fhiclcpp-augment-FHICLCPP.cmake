if (FHICLCPP)
  list(PREPEND FHICLCPP
    fhiclcpp::types
  )
  list(APPEND FHICLCPP
    art_plugin_support::plugin_config_macros
  )
endif()