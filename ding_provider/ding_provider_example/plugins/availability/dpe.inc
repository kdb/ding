<?php

// $Id$

/**
 * @file
 * Ding availability plugin.
 */

$plugin = array(
  'description' => t('Example availability plugin'),
  'version' => 1,
);

/**
 * Check availability of some items.
 *
 * @param $provider_ids
 *   Array of ids of items.
 *
 * @return
 *   Array of id => boolean.
 */
function ding_provider_example_availability_items($provider_ids) {
}

/**
 * Get availability details.
 *
 * For each item, return an array with the following keys:
 * - show_reservation_button: Boolean indicating whether an reservation
 *   button can be shown.
 * - available: Whether the item is available for reservation.
 * - reservable: Whether the item can be reserved at all.
 * - holdings: Array of strings of the locations of the material.
 *
 * @param $provider_ids
 *   Array of ids of items.
 *
 * @return
 *   Array of id => details array.
 */
function ding_provider_example_availability_details($provider_ids) {
}
