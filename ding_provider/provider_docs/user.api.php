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
