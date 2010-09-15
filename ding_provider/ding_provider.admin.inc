<?php

// $Id$

/**
 * Provides an overview of used plugins, and setting pages.
 */
function ding_provider_admin_form() {
  $plugin_types = array('user');
  foreach ($plugin_types as $type) {
    $form[$type] = array(
      #type' => 'fieldset',
      '#title' => t('Provider type @type', array('@type' => $type)),
    );

    $plugins = _ding_provider_get_plugins($type);
    $active_plugin = _ding_provider_get_plugin($type);

    $active_value = $active_plugin ? _ding_provider_plugin_description($active_plugin) : t('None');
    if ($active_plugin === FALSE) {
      $active_value = t('<em>Too many providers</em>');
    }
    $form[$type]['active'] = array(
      '#type' => 'item',
      '#title' => t('Currently using'),
      '#value' => $active_value,
    );

    if (sizeof($plugins) > 1) {
      if (isset($active_plugin['name'])) {
        unset($plugins[$active_plugin['name']]);
      }
      $form[$type]['inactive'] = array(
        '#type' => 'item',
        '#title' => t('Inactive'),
        '#value' => theme('item_list', array_map('_ding_provider_plugin_description', $plugins)),
      );
    }
  }

  $form['update'] = array(
    '#type' => 'submit',
    '#value' => 'Update',
  );

  return $form;
}

/**
 * Submit function.
 */
function ding_provider_admin_form_submit($form, &$form_state) {
  cache_clear_all("plugins:ding_provider:", 'cache', TRUE);
}