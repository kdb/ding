<?php

// $Id$

/**
 * @file
 * Ding reservation plugin.
 */

$plugin = array(
  'description' => t('Example reservation plugin'),
  'version' => 1,
);

/**
 */
function ding_provider_example_reservation_list($account) {
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
      // Materials number, optional. Is shown to the user.
      'materials_number' => 43125871,
      // Now, we could return a 'ting_object' here, but for efficiencys sake,
      // we note the id here, and load it in 'details'. For responses without
      // a ting reference, see the next entry.
      'provider_id' => 24907511, // Terry Pratchett, Monstrous regiment
      // Status of the reservation, 'active' or 'fetchable'
      'status' => 'active',
      // Description of pickup place.
      'pickup_branch' => 'Somewhere',
      // Creation date of reservation. Format: YYYY-MM-DD.
      'create_date' => strftime('%Y-%m-%d', strtotime('-7 days')),
      // Reservation valid from. Format: YYYY-MM-DD.
      'valid_from' => strftime('%Y-%m-%d', strtotime('-7 days')),
      // Reservation valid to. Format: YYYY-MM-DD.
      'valid_to' => strftime('%Y-%m-%d', strtotime('+30 days')),

      // For 'active' reservations:
      // Queue number. optional.
      'queue_number' => 3,

      // For 'fetchable' reservations, see the next entry.
    ),
    'euaeu' => array(
      'id' => 'euaeu', // string id...
      // Title of the item, optional.
      'title' => 'Hidden Empire',
      // Authors of the item, optional. Not shown if display_title is used.
      'authors' => 'Kevin J. Anderson',
      // Alternative title string, optional. Used if title is empty. If
      // neither 'title' nor 'display_title' is specified, an 'Title not
      // available' message is displayed.
      'display_title' => 'Kevin J. Anderson: Hidden Empire',
      // Status of the reservation, 'active' or 'fetchable'
      'status' => 'fetchable',
      // Description of pickup place.
      'pickup_branch' => 'Somewhere else',
      // Creation date of reservation. Format: YYYY-MM-DD.
      'create_date' => strftime('%Y-%m-%d', strtotime('-7 days')),
      // Reservation valid from. Format: YYYY-MM-DD.
      'valid_from' => strftime('%Y-%m-%d', strtotime('-7 days')),
      // Reservation valid to. Format: YYYY-MM-DD.
      'valid_to' => strftime('%Y-%m-%d', strtotime('+30 days')),

      // For 'fetchable' reservations:
      // Pickup number (?)
      'pickup_number' => 7,
      // Expiry of pickup.
      'pickup_expire_date' => strftime('%Y-%m-%d', strtotime('+4 days')),
    ),
  );
}

/**
 * Add details to a reservation for detailed display.
 *
 * The 'list' callback should only return minimal data, and leave out
 * data that would requiring querying additional services, for
 * speed. 'details' will be called when more data is needed, and should
 * populate extra data for display.
 *
 * Should implement internal per-request caching as it might be called
 * multiple times for the same loan, preferably using ctools_static.
 *
 * @param array $reservations
 *   The reservation to add details to.
 * @return
 *   The reservation with details added.
 */
function ding_provider_example_reservation_details($reservation) {
  $cache = &ctools_static(__FUNCTION__, array());
  if (!isset($cache[$reservation['id']])) {
    $reservation['extra'] = 'Tadaaa!';
    if ($reservation['provider_id'] and $object = ting_get_object_by_local_id($reservation['provider_id'])) {
      $reservation['ting_object'] = $object;
    }
    $cache[$reservation['id']] = $reservation;
  }
  return $cache[$reservation['id']];
}


/**
 * Create a reservation.
 *
 * Should use any available user defaults for interest period or
 * pickup branch.
 *
 * @param array $account
 *   The user to reserve for.
 * @return boolean
 *   Success or not.
 */
function ding_provider_example_reservation_create($account, $provider_id) {
  $cache = &ctools_static(__FUNCTION__, array());
  if (!isset($cache[$reservation['id']])) {
    $reservation['extra'] = 'Tadaaa!';
    if ($reservation['provider_id'] and $object = ting_get_object_by_local_id($reservation['provider_id'])) {
      $reservation['ting_object'] = $object;
    }
    $cache[$reservation['id']] = $reservation;
  }
  return $cache[$reservation['id']];
}
