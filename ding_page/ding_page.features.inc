<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function ding_page_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => 1);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function ding_page_node_info() {
  $items = array(
    'page' => array(
      'name' => t('Page'),
      'module' => 'features',
      'description' => t('A static CMS page. Used for hierarcihal content that seldom changes. Does not allow comments and is not by default displayed on the front page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
