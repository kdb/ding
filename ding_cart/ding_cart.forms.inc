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
  $creds = ding_library_user_get_credentials($account);

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
 */
function ding_cart_forms_cart_details_submit($form, &$form_state) {
  // For some reason ['#parents'][0] seems to be the only way to
  // reliably access the button's element name.
  switch ($form_state['clicked_button']['#parents'][0]) {
    case 'remove':
      db_query("DELETE FROM {ding_cart} WHERE uid = %d AND local_id IN (" . join(',', $form_state['values']['selected_items']) . ");", $form_state['values']['account']->uid);
      break;
  }
}

