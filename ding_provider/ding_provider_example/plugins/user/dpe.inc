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
  // are named MODULE_PLUGIN_FUNCTION, as shown below. Note that the
  // plugins name (the name of this file) is not included.
  'logged in' => 'ding_provider_example_user_logged_in',
  // Optional functions. They follow the same naming pattern.
  // Provider settings form.
  'settings form' => 'ding_provider_example_user_settings_form',
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
  $creds = ding_library_user_get_credentials($account);
  return $creds != DING_PROVIDER_AUTH_REQUIRED;
}

/**
 * Form callback for plugin settings.
 *
 * This is a regular form callback.
 */
function ding_provider_example_user_settings_form() {
  $form['ding_provider_example_credential_cache_duration'] = array(
    '#type' => 'select',
    '#title' => t('Credential cache duration'),
    '#description' => t('Determines how long the user’s library system credentials will be cached for. For the duration of the cache, the user will be able to access his personal information and interact with the library system without having to provide his credentials again.'),
    '#options' => array(
      300 => t('5 minutes'),
      900 => t('15 minutes'),
      1800 => t('30 minutes'),
      3600 => t('1 hour'),
      10800 => t('3 hours'),
      43200 => t('12 hours'),
      86400 => t('1 day'),
      259200 => t('3 days'),
      604800 => t('7 days'),
    ),
    '#default_value' => variable_get('ding_provider_example_credential_cache_duration', 1800),
  );

  return system_settings_form($form);
}

/**
 * Form callback for user library settings.
 *
 * This is a regular form callback.
 */
function ding_provider_example_user_library_settings_form() {
  $form = array();
  $creds = ding_library_user_get_credentials();
  if ($creds == DING_PROVIDER_AUTH_REQUIRED) {
    return $form;
  }

  // We don't have anything, but we know that the reservation plugin have.
  return $form;
}

/**
 * Authenticate user.
 *
 * @param string $name
 *   Username.
 * @param string $pass
 *   Password.
 *
 * @return array
 *   An array with the following keys:
 *   - result: Boolean, whether the authentication succeeded.
 *   - authname: String. Normally ding_library_user generates an authname for
 *     the authmap table, but the authenticator can override it with this
 *     (useful for migration).
 *   - creds: Credentials for the credentials cache. These will be retrievable
 *     with ding_library_user_get_credentials later. Can be anything
 *     serializable.
 *   - user: Array of extra properties for the user object. These will be
 *     available in any hook_user('login') functions. 'display_name' and
 *     'auth_mail' are recommended.
 */
function ding_provider_example_user_authenticate($name, $pass) {
  $return = array(
    'result' => FALSE,
  );

  if (!preg_match('/^dpe_.*$/', $name)) {
    return $return;
  }

  $return['result'] = TRUE;
  $return['creds'] = array(
    'name' => $name,
    'pass' => $pass,
  );

  return $return;
}

/**
 * Additional authentication handling.
 *
 * Authenticators might implement this function for additional setup
 * after the user has been logged in.
 *
 * Warning: The supplied user is not a reference, so modifying it wont
 * propagate. Also, only modify the global $user if the uid is the
 * same, as it might not be the same user.
 */
function ding_provider_example_user_authenticate_finalize($user) {
}

/**
 * Change user password.
 *
 * The caller has already checked that the user knows the old password.
 */
function ding_provider_example_user_password_change($user, $new_password) {
}