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
 * Get information on a user.
 *
 * Returns an array of user data, which as a minimum should contain
 * the keys 'user_id' and 'user_name', which is used to bind to a
 * Drupal user.
 *
 * @param object $account
 *   User object.
 * @param boolean $extended
 *   Optional, whether to return extended information.
 *
 * @return mixed
 *   An array of user information, or DING_PROVIDER_AUTH_REQUIRED if
 *   not authenticated.
 */
function ding_provider_example_user_get_info($account, $extended = FALSE) {
  $creds = _ding_provider_example_get_credentials($account);
  if ($creds == DING_PROVIDER_AUTH_REQUIRED) {
    return $creds;
  }
  return array('pet_name' => 'Rex');
}

/**
 * Update user information.
 *
 * Update the user information with the given values. Only set keys
 * should be considered, with NULL values signalling deletion.
 *
 * @param object $account
 *   Object of user to be updated.
 * @param array $new_info
 *   New user information.
 *
 * @return void
 */
function ding_provider_example_user_update_info($account, $new_info) {
  return TRUE;
}

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

  $form['branch'] => array(
    '#type' => 'select',
    '#title' => t('Select preferred branch'),
    '#options' => array(
      '1' => t('First branch'),
      '2' => t('Second branch'),
    ),
  );
}