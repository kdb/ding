<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function ding_base_ctools_plugin_api() {
  module_load_include('inc', 'ding_base', 'ding_base.defaults');
  $args = func_get_args();
  return call_user_func_array('_ding_base_ctools_plugin_api', $args);
}

/**
 * Implementation of hook_strongarm().
 */
function ding_base_strongarm() {
  module_load_include('inc', 'ding_base', 'ding_base.defaults');
  $args = func_get_args();
  return call_user_func_array('_ding_base_strongarm', $args);
}
