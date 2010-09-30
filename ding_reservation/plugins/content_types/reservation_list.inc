<?php
// $Id$

/**
 * @file reservation_list.inc
 * Provides reservation related ctools content types.
 */

$plugin = array(
  'title' => t('Library user profile'),
);

/**
 * Callback function to supply a list of content types.
 */
function ding_reservation_reservation_list_content_type_content_types() {
  $types = array(
    'ding_library_user_reservations_list' => array(
      'title' => t('Reservation list'),
      'description' => t('A simple list of the user’s reservations.'),
    ),
    'ding_library_user_reservations_details' => array(
      'title' => t('Reservation details'),
      'description' => t('A detailed list of the user’s reservations.'),
    ),
  );

  // Append our default settings to every type.
  foreach ($types as $name => $type) {
    $types[$name] += array(
      'category' => t('Library user profile'),
      'required context' => new ctools_context_required(t('user'), 'user'),
    );
  }

  return $types;
}

/**
 * Return the pane contents.
 */
function ding_reservation_reservation_list_content_type_render($subtype, $conf, $panel_args, $context) {
  $account = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block = new stdClass();

  if (ding_provider_invoke('user', 'logged in', $account)) {
    switch ($subtype) {

      case 'ding_library_user_reservations_list':
        $list = ding_provider_invoke('reservation', 'list', $account);
        if ($list != DING_PROVIDER_AUTH_REQUIRED) {
          $block->title = t('Your reservations');
          $block->content = theme('ding_library_user_record_list', 'reservation', $list);
        }
        break;

      case 'ding_library_user_reservations_details':
        module_load_include('forms.inc', 'ding_library_user');
        $list = ding_provider_invoke('reservation', 'list', $account);
        $block->title = t('Your reservations');
        $block->content = drupal_get_form('ding_reservation_list_form', $account, $list);
        break;
    }
  }

  return $block;
}

/**
 * Return the pane titles.
 */
function ding_reservation_reservation_list_content_type_admin_title($subtype, $conf, $context) {
  switch($subtype) {
    case 'ding_library_user_reservations_list':
      return t('“@s”: Reservation list', array('@s' => $context->identifier));
    case 'ding_library_user_reservations_details':
      return t('“@s”: Reservation details', array('@s' => $context->identifier));
  }
}

/**
 * Returns an edit form for the content type.
 *
 * We're not actually doing anything here, but we need to have this hook
 * for this content type to work.
 */
function ding_reservation_reservation_list_content_type_edit_form(&$form, &$form_state) {
  return $form;
}

