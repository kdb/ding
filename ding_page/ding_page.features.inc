<?php

/**
 * Implementation of hook_content_default_fields().
 */
function ding_page_content_default_fields() {
  module_load_include('inc', 'ding_page', 'ding_page.defaults');
  $args = func_get_args();
  return call_user_func_array('_ding_page_content_default_fields', $args);
}

/**
 * Implementation of hook_ctools_plugin_api().
 */
function ding_page_ctools_plugin_api() {
  module_load_include('inc', 'ding_page', 'ding_page.defaults');
  $args = func_get_args();
  return call_user_func_array('_ding_page_ctools_plugin_api', $args);
}

/**
 * Implementation of hook_default_page_manager_handlers().
 */
function ding_page_default_page_manager_handlers() {
  module_load_include('inc', 'ding_page', 'ding_page.defaults');
  $args = func_get_args();
  return call_user_func_array('_ding_page_default_page_manager_handlers', $args);
}

/**
 * Implementation of hook_strongarm().
 */
function ding_page_strongarm() {
  module_load_include('inc', 'ding_page', 'ding_page.defaults');
  $args = func_get_args();
  return call_user_func_array('_ding_page_strongarm', $args);
}
