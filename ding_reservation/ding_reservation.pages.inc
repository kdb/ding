<?php
// $Id$

/**
 * Reserve an item immediately.
 */
function ding_reservation_page_reserve_item($item_id, $reservable = NULL) {
  global $user;
  $branch_names = module_invoke_all('ding_library_get_branches', FALSE);
  module_load_include('client.inc', 'ting');
  $is_ahah = ding_reservation_detect_ahah();
  // Get object, enrich if not ahah.
  $object = ting_get_object_by_local_id($item_id, !$is_ahah);
  if (!$object) {
    return drupal_not_found();
  }

  $user = $GLOBALS['user'];

  $reservation_exists = ding_provider_invoke('reservation', 'list', $user, $item_id);

  // If client wants JSON (usually AJAX requests), let's give it to them.
  if ($is_ahah) {
    $reply = array();
    if (!$user->uid || !ding_provider_invoke('user', 'logged in', $user)) {
      $reply['status'] = 'error';
      $reply['error_type'] = 'not-logged-in';
      $reply['title'] = t('Not logged in');
      $reply['message'] = t('You have to log in to make reservations.');
    }
    elseif (!isset($user->preferred_branch) || !isset($branch_names[$user->preferred_branch])) {
      $reply['status'] = 'error';
      $reply['error_type'] = 'no-preferred-branch';
      $reply['title'] = t('Preferred branch not set');
      $reply['message'] = t('To make quick reservations, you need to have a preferred branch set. You can set one up at your !profile_settings.',
        array('!profile_settings' => l(t('profile settings page'), 'user/' . $user->uid . '/profile')
      ));

    }
    elseif ($reservation_exists && $reservation_exists != DING_PROVIDER_AUTH_REQUIRED) {
      $reply['status'] = 'error';
      $reply['error_type'] = 'reservation-exists';
      $reply['title'] = t('Reservation exists');
      $reply['message'] = t('@item is already reserved.', array('@item' => $object->title));
    }
    else {
      $reservation = ding_provider_invoke('reservation', 'create', $user, $item_id, array('pickup_branch' => $user->preferred_branch));

      if ($reservation && $reservation != DING_PROVIDER_AUTH_REQUIRED) {
        $reply['status'] = 'success';
        $reply['title'] = t('Success');
        $reply['message'] = t('You have reserved @item and will be notified when its ready for pickup at @branch.', array(
          '@item' => $object->title,
          '@branch' => $branch_names[$user->preferred_branch],
        ));
        $reply['cart_link'] = url('user/' . $user->uid . '/status');
      }
      else {
        $reply['status'] = 'error';
        $reply['error_type'] = 'reservation-error';
        $reply['title'] = t('Reservation error');
        $reply['message'] = t('An error occurred while reserving item. Please try again.');
      }
    }

    // We are returning JavaScript, so tell the browser.
    drupal_set_header('Content-Type: application/json; charset=utf-8');
    echo json_encode($reply);
    return;
  }

  // Ensure that we're authenticated with the backend, redirect if not.
  ding_provider_invoke('user', 'logged in', $user, TRUE);
  if ($reservation_exists && $reservation_exists != DING_PROVIDER_AUTH_REQUIRED) {
    drupal_set_message(t('@item is already reserved.', array('@item' => $object->title)), 'warning');
    return drupal_goto('user/' . $user->uid . '/status');
  }

  return drupal_get_form('ding_reservation_confirm_form', $user, $object);
}

/**
 * Ding reservation confirmation form.
 *
 * @param array $form_state
 *    Drupal form API state.
 * @param stdClass $account
 *    Drupal user account.
 * @param TingClientObject $object
 *    The object we're acting on.
 */
function ding_reservation_confirm_form(&$form_state, $account, $object) {
  // Send some form values along for the submit step.
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['object'] = array(
    '#type' => 'value',
    '#value' => $object,
  );

  $form['dest_url'] = array(
    '#type' => 'value',
    '#value' => $urls[$action],
  );

  return confirm_form($form, t('Reserve %book immediately?', array('%book' => $object->title)), 'user/' . $account->uid . '/status', '');
}

/**
 * Submit handler for Ding cart confirmation form.
 * TODO Not all library systems may support reservations - move/refactor this
 */
function ding_reservation_confirm_form_submit($form, &$form_state) {
  $reservation = ding_provider_invoke('reservation', 'create', $form_state['values']['account'], $form_state['values']['object']->localId, array('pickup_branch' => $form_state['values']['account']->preferred_branch));

  if ($reservation && $reservation != DING_PROVIDER_AUTH_REQUIRED) {
    drupal_set_message(t('@item is now on your reservation list.', array('@item' => $form_state['values']['object']->title)));
  }
  else {
    drupal_set_message(t('Reservation of @item failed.', array('@item' => $form_state['values']['object']->title)));
  }
  break;

  $form_state['redirect'] = $form_state['values']['dest_url'];
}

/**
 * Detect if a request wants a JSON-response, usually for AHAH.
 */
function ding_reservation_detect_ahah() {
  return (isset($_SERVER['HTTP_ACCEPT']) && (strpos($_SERVER['HTTP_ACCEPT'], 'application/json')) || (strpos($_SERVER['HTTP_ACCEPT'], 'text/javascript')));
}