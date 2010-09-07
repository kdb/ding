<?php
// $Id$

/**
 * User cart details form.
 *
 * @param array $form_state
 *    Drupal form API state.
 * @param stdClass $account
 *    The user object for the cart we're looking at.
 * @return array
 */
function ding_cart_forms_cart_details(&$form_state, $account) {
  $form = array();

  // Get the credentials to make sure this session is properly
  // authenticated before displaying the cart contents.
  $creds = module_invoke_all('ding_user_has_credentials', $account);

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['cart_data'] = array(
    '#type' => 'value',
    '#value' => array(),
  );

  // Fetch the cart data from the database.
  $query = db_query("SELECT * FROM {ding_cart} WHERE uid = %d;", $account->uid);
  $local_ids = array();
  while ($row = db_fetch_array($query)) {
    $form['cart_data']['#value'][$row['local_id']] = $row;
    $local_ids[] = $row['local_id'];
  }

  $details = module_invoke_all('ding_get_object_details', $local_ids);

  $form['cart'] = array(
    // Set as tree to get all of our checkboxes as an subarray of values
    // in the form_state['values'] array.
    '#tree' => TRUE,
  );

  foreach ($form['cart_data']['#value'] as $item) {
    $form['cart'][$item['local_id']] = array(
      '#type' => 'checkbox',
    );
  }

  $interest_period = (isset($account->interest_period) && $account->interest_period > 10) ? $account->interest_period : DING_USER_DEFAULT_INTEREST_PERIOD;

  // TODO Not all library systems may support reservations - refactor this to use hook or form_alter
  $form['expiry'] = array(
    '#type' => 'date_popup',
    '#title' => t('Not interested after'),
    '#date_format' => DATE_FORMAT_DATE,
    '#date_year_range' => '-0:+2',
    '#default_value' => date('Y-m-d', $_SERVER['REQUEST_TIME'] + $interest_period * 86400),
  );

  // TODO Not all library systems may support reservations - refactor this to use hook or form_alter
  $form['pickup_branch'] = array(
    '#type' => 'select',
    '#title' => t('Pickup branch'),
    '#options' => module_invoke_all('ding_get_reservation_branches'),
    '#default_value' => $account->preferred_branch,
  );

  $form['remove'] = array(
    '#type' => 'submit',
    '#value' => t('Remove selected'),
    '#attributes' => array('class' => 'remove-button'),
  );

  // TODO Not all library systems may support reservations - refactor this to use hook or form_alter
  $form['reserve'] = array(
    '#type' => 'submit',
    '#value' => t('Reserve selected'),
    '#attributes' => array('class' => 'reserve-button'),
  );

  return $form;
}

/**
 * Validation for cart details form.
 */
function ding_cart_forms_cart_details_validate($form, &$form_state) {
  $selected_items = array_keys(array_filter($form_state['values']['cart']));

  if (empty($selected_items)) {
    form_set_error('items', t('Please select one or more items from the cart.'));
  }
  $form_state['values']['selected_items'] = $selected_items;

  // Strip off the time part of the return value.
  $form_state['values']['expiry'] = substr($form_state['values']['expiry'], 0, 10);

  if (date('Y-m-d', $_SERVER['REQUEST_TIME']) >= $form_state['values']['expiry']) {
    form_set_error('expiry', t('Date must be in the future.'));
  }
}


/**
 * Submit handler for cart details form.
 * TODO Not all library systems may support reservations - refactor this to use hook or form_alter
 */
function ding_cart_forms_cart_details_submit($form, &$form_state) {
  // For some reason ['#parents'][0] seems to be the only way to
  // reliably access the button's element name.
  switch ($form_state['clicked_button']['#parents'][0]) {
    case 'reserve':
      $creds = module_invoke_all('ding_user_get_credentials', $form_state['values']['account']);

      foreach ($form_state['values']['selected_items'] as $local_id) {
        $reservation = module_invoke_all('ding_create_reservation', $creds, $local_id, $form_state['values']['pickup_branch'], $form_state['values']['expiry']);

        // If reservation was successful, remove the item from the cart.
        if (in_array(TRUE, $reservation)) {
          db_query("DELETE FROM {ding_cart} WHERE uid = %d AND local_id = '%s';", array(
            ':uid' => $form_state['values']['account']->uid,
            ':local_id' => $local_id,
          ));
        }
        else {
          drupal_set_message(t('Reservation of “@title” failed. It may be on your reservation list already.', array('@title' => $form['cart_data']['#value'][$local_id]['title'])), 'error');
        }
      }
      break;
    case 'remove':
      db_query("DELETE FROM {ding_cart} WHERE uid = %d AND local_id IN (" . join(',', $form_state['values']['selected_items']) . ");", $form_state['values']['account']->uid);
      break;
  }
}

