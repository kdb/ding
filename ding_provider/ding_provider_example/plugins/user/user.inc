<?php

// $Id$

/**
 * @file
 * Ding user plugin.
 */

$plugin = array(
  // Plugin description. Shown in the backend.
  'description' => t('Example user provider'),
  // Version compatibility.
  'version' => 1,
  // Required functions. Their definition here can be omitted if they
  // are named MODULE_PLUGIN_FUNCTION, as shown below.
  'get info' => 'ding_provider_example_user_get_info',
  'update info' => 'ding_provider_example_user_update_info',
  // Optional functions. They follow the same naming pattern.
  'library settings form' => 'ding_provider_example_user_library_settings_form',
);

/**
 * Returns whether the account is currently 'logged in' to the user
 * backend.
 *
 * If not, we'll need to authenticate before we can do anything.
 *
 * @param object $account
 *   The user to check.
 *
 * @return boolean
 *   TRUE if the account is authenticated.
 */
function ding_provider_example_user_logged_in($account) {
  $creds = _ding_provider_example_get_credentials($account);
  return $creds != DING_PROVIDER_AUTH_REQUIRED;
}

/**
 * Form callback for library settings.
 *
 * This is a regular form callback.
 */
function ding_provider_example_user_library_settings_form() {
  $form = array();
  $creds = _ding_provider_example_get_credentials();
  if ($creds == DING_PROVIDER_AUTH_REQUIRED) {
    return $form;
  }

  $form['branch'] = array(
    '#type' => 'select',
    '#title' => t('Select preferred branch'),
    '#options' => array(
      '1' => t('First branch'),
      '2' => t('Second branch'),
    ),
  );
}