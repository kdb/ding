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
 * @return array
 *   An array of user information.
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
