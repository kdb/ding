<?php

// $Id$

/**
 * @file
 * Ding loan plugin.
 */

$plugin = array(
  // Plugin description. Shown in the backend.
  'description' => t('Xxx loan plugin'),
  // Version compatibility.
  'version' => 1,
  // Required functions. Their definition here can be omitted if their
  // named as the examples, where MODULE is the defining module and
  // PLUGIN is the name of the plugin (the name of the file, minus
  // '.inc').
  'get loans' => 'MODULE_PLUGIN_loan_get_loans',
  'renew loan' => 'MODULE_PLUGIN_loan_renew_loan',
);

/**
 */
function MODULE_PLUGIN_loan_get_loans($account, $redirect = FALSE) {

}

/**
 */
function MODULE_PLUGIN_loan_renew_loan($account, $loan) {

}
