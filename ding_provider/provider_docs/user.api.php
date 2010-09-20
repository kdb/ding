<?php

// $Id$

/**
 * @file
 * Ding user plugin.
 */

$plugin = array(
  // Plugin description. Shown in the backend.
  'description' => t('Xxx user plugin'),
  // Version compatibility.
  'version' => 1,
  // Required functions. Their definition here can be omitted if their
  // named as the examples, where MODULE is the defining module and
  // PLUGIN is the name of the plugin (the name of the file, minus
  // '.inc').
  'get info' => 'MODULE_PLUGIN_user_get_info',
  'update info' => 'MODULE_PLUGIN_user_update_info',
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
function MODULE_PLUGIN_user_get_info($account, $extended = FALSE) {

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
function MODULE_PLUGIN_user_update_info($account, $new_info) {

}

/**
 * Returns whether the account is currently 'logged in' to the user
 * backend.
 *
 * If not, we'll need to authenticate before we can do anything.
 *
 * @param object $account
 *   The user to check.
 * @param boolean $redirect
 *   Whether it's OK to redirect to log the user in.
 *
 * @return boolean
 *   TRUE if the account is authenticated.
 */
function MODULE_PLUGIN_user_logged_in($account, $redirect = FALSE) {

}
