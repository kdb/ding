<?php
// $Id$

/**
 * Alma authentication form.
 *
 * Only used when a logged in user requires access to data from Alma and
 * his Alma credentials are not cached.
 *
 * @param array $form_state
 *    Drupal's form state.
 * @param stdClass $account
 *    User account object.
 * @param mixed $explanation
 *    Explanation for the login form. NULL for the default message.
 * @param mixed $redirect
 *    Redirect destination – boolean TRUE for default destination
 *    (user's profile page).
 */
function ding_library_user_forms_authenticate($form_state, $account, $explanation=NULL, $redirect=TRUE) {
  $form = array();

  $form['redirect'] = array(
    '#type' => 'value',
    '#value' => $redirect,
  );

  // Strict NULL check to allow an empty explanation string.
  if (is_null($explanation)) {
    $explanation = t('To access your personal information, you need to authenticate.');
  }

  $form['explanation'] = array(
    '#value' => $explanation,
  );

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['borr_card'] = array(
    '#type' => 'password',
    '#title' => t('Borrower card'),
    '#required' => TRUE,
  );

  $form['pin_code'] = array(
    '#type' => 'password',
    '#title' => t('PIN code'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Log in'),
  );

  return $form;
}

/**
 * Validation for authentication form.
 */
function ding_library_user_forms_authenticate_validate($form, &$form_state) {
  // Lookup user data
  $data = module_invoke_all('ding_library_user_get_info', array('user_id' => $form_state['values']['user_id'], 'password' => $form_state['values']['password']));
  $data = array_shift($data);
  
  // If we got no data or account IDs do not match, it usually means that the
  // user supplied the wrong credentials.
  if (!$data || $data['user_id'] != $form_state['values']['account']->library_user_id) {
    form_set_error('user_id', t('Invalid user name or password'));
  }
  else {
    // Validation succeeded, so cache the correct credentials.
    ding_library_user_cache_credentials($card, $form_state['values']['user_id'], $form_state['values']['password']);
  }
}

/**
 * Submit handler for authentication form.
 */
function ding_library_user_forms_authenticate_submit($form, &$form_state) {
  // If we do not have a destination already, use the redirect value.
  if (!isset($_REQUEST['destination']) && $form_state['values']['redirect']) {
    // If redirect is boolean TRUE, use the default redirect.
    if (is_bool($form_state['values']['redirect'])) {
      $form_state['redirect'] = 'user/' . $form_state['values']['account']->uid . '/profile';
    }
    else {
      // Otherwise, assume that the redirect value is a sane destination.
      $form_state['redirect'] = $form_state['values']['redirect'];
    }
  }
}

/**
 * Change password form.
 */
function ding_library_user_forms_change_password($form_state, $account) {
  $password_length = variable_get('ding_library_user_password_length', 4);
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['current_password'] = array(
    '#type' => 'password',
    '#title' => t('Current password'),
    '#required' => TRUE,
    '#maxlength' => $password_length,
    '#size' => $password_length,
  );

  $form['new_password'] = array(
    '#type' => 'password',
    '#title' => t('New password'),
    '#required' => TRUE,
    '#maxlength' => $password_length,
    '#size' => $password_length,
  );

  $form['new_password_confirm'] = array(
    '#type' => 'password',
    '#title' => t('Repeat new password'),
    '#required' => TRUE,
    '#maxlength' => $password_length,
    '#size' => $password_length,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save new password'),
  );

  return $form;
}

/**
 * Validation for library password changing form.
 */
function ding_library_user_forms_change_password_validate($form, &$form_state) {
  $password_length = variable_get('ding_library_user_password_length', 4);
  $creds = ding_library_user_get_credentials($form_state['values']['account']);
  if ($form_state['values']['current_password'] != $creds['password']) {
    form_set_error('current_password', t('Please provide your current password.'));
  }

  if ($form_state['values']['current_password'] == $form_state['values']['new_password']) {
    form_set_error('new_pin', t('The new password must be different than the current password.'));
  }

  if ($form_state['values']['new_password_confirm'] != $form_state['values']['new_password']) {
    form_set_error('new_password_confirm', t('Please input your new password twice for confirmation.'));
  }

  if (!ctype_digit($form_state['values']['new_password']) || strlen($form_state['values']['new_password']) != $password_length) {
    form_set_error('new_password', t('The new password must be @num numerical digits.', array('@num' => $password_length)));
  }
}

/**
 * Submit handler for the password changing form.
 */
function ding_library_user_forms_change_password_submit($form, &$form_state) {
  $creds = ding_library_user_get_credentials($form_state['values']['account']);
  module_invoke_all('ding_library_user_update_info', $creds, array('password' => $form_state['values']['new_password']));

  // Cache the new password
  ding_library_user_cache_credentials($form_state['values']['account'], $creds['user_id'], $form_state['values']['new_password']);

  // If we do not have a destination already, go to the profile.
  if (!isset($_REQUEST['destination'])) {
    $form_state['redirect'] = 'user/' . $form_state['values']['account']->uid . '/profile';
  }
}

/**
 * User library settings form.
 */
function ding_library_user_forms_library_settings($form_state, $account) {
  $creds = ding_library_user_get_credentials($account);
  $info = module_invoke_all('ding_library_user_get_info', $creds, TRUE);
  $account->user_info = $info;
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#weight' => 50,
  );

  return $form;
}

/**
 * Reservation details form.
 */
function ding_library_user_forms_reservation_details($form_state, $account, $records) {
  $cancel_fetchable = (boolean) variable_get('ding_library_user_cancel_fetchable_reservable', TRUE);
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['reservation_data'] = array(
    '#type' => 'value',
    '#grouped' => array(),
    '#value' => array(),
  );

  $form['reservations'] = array(
    '#tree' => TRUE,
  );

  foreach ($records as $reservation) {
    $form['reservation_data']['#value'][$reservation['id']] = $reservation;
    $form['reservation_data']['#grouped'][$reservation['status']][$reservation['id']] = $reservation;

    if ($reservation['status'] == 'active' || ($cancel_fetchable && $reservation['status'] == 'fetchable')) {
      $form['reservations'][$reservation['id']] = array(
        '#type' => 'checkbox',
      );
    }
  }

  $form['remove'] = array(
    '#type' => 'submit',
    '#value' => t('Delete selected reservations'),
    '#attributes' => array('class' => 'cancel-button'),
  );

  $form['new_expiry'] = array(
    '#type' => 'date_popup',
    '#title' => t('Valid to'),
    '#date_format' => DATE_FORMAT_DATE,
    '#date_year_range' => '-0:+2',
  );

  $form['new_pickup_branch'] = array(
    '#type' => 'select',
    '#title' => t('New pickup branch'),
    '#options' => module_invoke_all('ding_library_get_reservation_branches'),
  );

  $form['update'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes for selected reservations'),
    '#attributes' => array('class' => 'update-button'),
  );

  return $form;
}

/**
 * Reservation details form validation.
 */
function ding_library_user_forms_reservation_details_validate($form, &$form_state) {
	$data = $form_state['values'];

	// Prevent users from updating reservations which is ready for pickup at a branch
	if (strpos($form_state['clicked_button']['#attributes']['class'], 'update-button') !== FALSE) {
	  $branches = module_invoke_all('ding_library_get_branches');
		foreach ($data['reservations'] as $id => $selected) {
	    if ($selected == 1) {
	    	$reservation = $data['reservation_data'][$id]; 
	      if (($reservation['status'] == 'fetchable') && // Detect fetable reservations...
	          (($reservation['pickup_branch'] != $data['new_pickup_branch']) || // where the user tried to alter pickup branch
	          $data['new_expiry'])) { // or alter expiry date
          form_set_error('reservations]['.$id, t('You can not perform changes to reservations which are ready for pick up'));
	      }
	    }
	  }
	}
  
	if (!empty($data['new_expiry'])) {
    list($year, $month, $day) = explode('-', trim($data['new_expiry']));
    if (!checkdate($month, $day, $year)) {
      form_set_error('new_expiry', t('Invalid date'));
    }

    $timestamp = strtotime($data['new_expiry']);

    if ($timestamp <= $_SERVER['REQUEST_TIME']) {
      form_set_error('new_expiry', t('Date must be later than today.'));
    }
    else {
      form_set_value($form['new_expiry'], format_date($timestamp, 'custom', 'Y-m-d'), $form_state);
    }
    
  }
}

/**
 * Reservation details form submit handler.
 */
function ding_library_user_forms_reservation_details_submit($form, &$form_state) {
  $selected = array();
  $reservations = $form_state['values']['reservation_data'];

  foreach ($form_state['values']['reservations'] as $id => $val) {
    if ($val == 1) {
      $selected[] = $id;
    }
  }

  switch ($form_state['clicked_button']['#attributes']['class']) {
    case 'update-button':
      $changes = array();
      if (!empty($form_state['values']['new_expiry'])) {
        $changes['valid_to'] = $form_state['values']['new_expiry'];
      }

      if (!empty($form_state['values']['new_pickup_branch'])) {
        $changes['pickup_branch'] = $form_state['values']['new_pickup_branch'];
      }

      if (!empty($selected) && !empty($changes)) {
        foreach ($selected as $id) {
          module_invoke_all('ding_library_user_update_reservation', $form_state['values']['account'], $reservations[$id], $changes);
        }
      }
      break;
      
    case 'cancel-button':
      foreach ($selected as $id) {
      	module_invoke_all('ding_library_user_delete_reservation', $form_state['values']['account'], $reservations[$id]);
      }
      break;
  }
}

/**
 * Loan details form.
 */
function ding_library_user_forms_loan_details($form_state, $account, $records) {
  $grouped = _ding_library_user_forms_group_loans($records);
  $renew_buttons = FALSE;
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['loan_data'] = array(
    '#type' => 'value',
    '#grouped' => $grouped,
    '#value' => $records,
  );

  $form['loans'] = array(
    '#tree' => TRUE,
  );

  foreach ($records as $loan) {
    if ($loan['is_renewable']) {
      $renew_buttons = TRUE;
      $form['loans'][$loan['id']] = array(
        '#type' => 'checkbox',
      );
    }
  }


  if ($renew_buttons) {
    $form['renew'] = array(
      '#type' => 'submit',
      '#value' => t('Renew selected'),
    );

    $form['renew_all'] = array(
      '#type' => 'submit',
      '#value' => t('Renew all'),
      '#attributes' => array('class' => 'renew-all-button'),
    );
  }

  return $form;
}

/**
 * Loan details submit handler.
 */
function ding_library_user_forms_loan_details_submit($form, $form_state) {
  $selected = array();

  switch ($form_state['clicked_button']['#parents'][0]) {
    case 'renew':
      foreach ($form_state['values']['loans'] as $key => $value) {
        if ($value) {
          $selected[] = $key;
        }
      }
      break;
    case 'renew_all':
      foreach ($form_state['values']['loan_data'] as $loan) {
        if ($loan['is_renewable']) {
          $selected[] = $loan['id'];
        }
      }
      break;
  }

  $creds = ding_library_user_get_credentials($form_state['values']['account']);
  module_invoke_all('ding_renew_loans', $creds, $selected);
}

/**
 * Helper function to group loans by their due dates.
 */
function _ding_library_user_forms_group_loans($records) {
  $groups = array();
  $today = date('Y-m-d', $_SERVER['REQUEST_TIME']);

  foreach ($records as $loan) {
    if ($loan['due_date'] < $today) {
      $groups['overdue'][$loan['id']] = $loan['id'];
    }
    elseif ($loan['due_date'] == $today) {
      $groups['due'][$loan['id']] = $loan['id'];
    }
    else {
      $groups[$loan['due_date']][$loan['id']] = $loan['id'];
    }
  }
  return $groups;
}

/**
 * Debts details form.
 */
function ding_library_user_forms_debts_details(&$form_state, $account, $debt_data) {
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['debt_data'] = array(
    '#type' => 'value',
    '#value' => $debt_data,
  );

  $form['debts'] = array(
    '#tree' => TRUE,
  );

  foreach ($debt_data['debts'] as $debt) {
    $form['debts'][$debt['id']] = array(
      '#type' => 'checkbox',
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Pay now'),
  );

  return $form;
}

/**
 * Validation for debts details form.
 */
function ding_library_user_forms_debts_details_validate($form, &$form_state) {
  $selected_debts = array_keys(array_filter($form_state['values']['debts']));

  if (empty($selected_debts)) {
    form_set_error('debts', t('Please select one or more debts to pay.'));
  }
  $form_state['values']['selected_debts'] = $selected_debts;

  $total = 0.0;

  foreach ($selected_debts as $debt_id) {
    $total += (float) $form_state['values']['debt_data']['debts'][$debt_id]['amount'];
  }

  if ($total <= 0.0) {
    form_set_error('debts', t('Payment must be larger than zero.'));
  }
  $form_state['values']['total_amount'] = $total;
}

