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
  // As we don't really have a backend, we just fake it.
  return array(
    // The keys should correspond with the items ids.
    '123' => array(
      // Id of the loan/reservation. Used as identifier. No assumptions are
      // made about it, however it should be a sting value suitable for values
      // in forms and the like.
      'id' => '123',
      // Materials number. Is shown to the user.
      'materials_number' => 1238141,
      // Date of the loan, optional. Format: YYYY-MM-DD.
      'loan_date' => '2010-09-15',
      // Due date of the loan, optional. Format: YYYY-MM-DD.
      'due_date' => strftime('%Y-%m-%d', strtotime('yesterday')),
      // Whether this loan is renewable.
      'is_renewable' => TRUE,
      // Whether this is a remote loan.
      'remote_loan' => FALSE,
      // Now, we could return a 'ting_object' here, but for efficiencys sake,
      // we note the id here, and load it in 'details'. For responses without
      // a ting reference, see the next entry.
      'provider_id' => 41406828, // Larry Niven, The Ringworld throne
    ),
    'euaeu' => array(
      'id' => 'euaeu', // string id...
      // Title of the item, optional.
      'title' => 'Ark',
      // Authors of the item, optional. Not shown if display_title is used.
      'authors' => 'Stephen Baxter',
      // Alternative title string, optional. Used if title is empty. If
      // neither 'title' nor 'display_title' is specified, an 'Title not
      // available' message is displayed.
      'display_title' => 'Stephen Baxter: Ark',
      'loan_date' => '2010-09-15',
      'due_date' => strftime('%Y-%m-%d', strtotime('+3 days')),
      'is_renewable' => TRUE,
    ),
    431 => array(
      'id' => 431, // not string for testing purposes
      'provider_id' => 43389122, // Alastair Reynolds, Absolution gap
      'loan_date' => '2010-09-15',
      'due_date' => strftime('%Y-%m-%d', strtotime('today')),
      'is_renewable' => TRUE,
      'remote_loan' => TRUE,
    ),
    '4364625' => array(
      'id' => '4364625',
      'provider_id' => 41168315, // Douglas Adams, The hitch hiker's guide to the galaxy
      // 'loan_date' missing for testing
      'due_date' => strftime('%Y-%m-%d', strtotime('+3 days')),
      'is_renewable' => TRUE,
    ),
    '4526725' => array(
      'id' => '4526725',
      'provider_id' => 44259362, // Neal Asher, Brass man
      'loan_date' => '2010-09-15',
      'due_date' => strftime('%Y-%m-%d', strtotime('+3 days')),
    ),
    '16164515' => array(
      'id' => '16164515',
      'provider_id' => 42021377, // Iain Banks, Excession
      'loan_date' => '2010-09-15',
      // 'due_date' missing for testing
    ),
  );
}

/**
 * Add details to a loan for detailed display.
 *
 * The 'get loans' callback should only return minimal data, and leave out
 * data that would requiring querying additional services, for
 * speed. 'details' will be called when more data is needed, and should
 * populate extra data for display.
 *
 * Should implement internal per-request caching as it might be called
 * multiple times for the same loan, preferably using ctools_static.
 *
 * @param array $loan
 *   The loan to add details to.
 * @return
 *   The loan with details added.
 */
function ding_provider_example_loan_details($loan) {
  $cache = &ctools_static(__FUNCTION__, array());
  if (!isset($cache[$loan['id']])) {
    $loan['extra'] = 'Tadaaa!';
    if ($loan['provider_id'] and $object = ting_get_object_by_local_id($loan['provider_id'])) {
      $loan['ting_object'] = $object;
    }
    $cache[$loan['id']] = $loan;
  }
  return $cache[$loan['id']];
}

/**
 * Renew loans.
 *
 * @param object $account
 *   The user for which to renew.
 * @param array $loan_ids
 *   Ids of the loans to renew.
 * @return boolean
 *   Whether all loans were renewed.
 */
function ding_provider_example_loan_renew_loans($account, $loan_ids) {
  return TRUE;
}
