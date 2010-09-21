<?php

// $Id$

/**
 * @file
 * Ding loan plugin.
 */

$plugin = array(
  'description' => t('Example loan plugin'),
  'version' => 1,
);

/**
 */
function ding_provider_example_loan_get_loans($account) {
  $creds = _ding_provider_example_get_credentials($account);
  if ($creds == DING_PROVIDER_AUTH_REQUIRED) {
    return $creds;
  }
  // As we doesn't really have a backend, we just fake it.
  // Eeeh..
  return array();
}

/**
 */
function ding_provider_example_loan_renew_loan($account, $loan) {
  $creds = _ding_provider_example_get_credentials($account);
  $reservations = alma_client_get_reservations($creds['user_id'], $creds['password/email/etc']);
  return $reservations;
}
