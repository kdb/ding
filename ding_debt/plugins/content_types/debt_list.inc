<?php
// $Id$

/**
 * @file debt_list.inc
 * Provides debt related ctools content types.
 */

$plugin = array(
  'title' => t('Library user profile'),
);

/**
 * Callback function to supply a list of content types.
 */
function ding_debt_debt_list_content_type_content_types() {
  $types = array(
    'ding_debt_notice' => array(
      'title' => t('User debts notice'),
      'description' => t('Provides a short noice to the user about his library debts.'),
    ),
    'ding_debt_list_form' => array(
      'title' => t('User debts details'),
      'description' => t('Provides the full list of user debts, with an option to pay them.'),
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
function ding_debt_debt_list_content_type_render($subtype, $conf, $panel_args, $context) {
  $account = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block = new stdClass();

  if (ding_provider_invoke('user', 'logged in', $account)) {
    switch ($subtype) {
      case 'ding_debt_notice':
        $debts = ding_provider_invoke('debt', 'list', $account);
        if (count($debts) == 1) {
          $debt = array_pop($debts);
          $block->content = t('You owe @amount for “%note”. !pay_link.', array(
            '@amount' => $debts['amount'],
            '%note' => trim($debts['display_title']),
            '!pay_link' => l(t('Pay now'), 'user/' . $account->uid . '/debts'),
          ));
        }
        elseif (count($debts) > 1) {
          $total = _ding_debt_list_get_total($debts);
          reset($debts);
          $debt = current($debts);
          $block->content = t('You owe @amount for “%note” and !others_link. !pay_link.', array(
            '@amount' => $total,
            '%note' => trim($debt['display_title']),
            '!others_link' => l(format_plural(count($debts) - 1, 'one other', '@count others'),
                                'user/' . $account->uid . '/debts'),
            '!pay_link' => l(t('Pay now'), 'user/' . $account->uid . '/debts'),
          ));
        }
        break;

      case 'ding_debt_list_form':
        $data = ding_provider_invoke('debt', 'list', $account);
        $block->content = drupal_get_form('ding_debt_list_form', $account, $data);
        break;
    }
  }

  return $block;
}

/**
 * Return the pane titles.
 */
function ding_debt_debt_list_content_type_admin_title($subtype, $conf, $context) {
  switch($subtype) {
    case 'ding_debt_notice':
      return t('“@s”: Debts notice', array('@s' => $context->identifier));
    case 'ding_debt_list_form':
      return t('“@s”: Debts list', array('@s' => $context->identifier));
  }
}

/**
 * Returns an edit form for the content type.
 *
 * We're not actually doing anything here, but we need to have this hook
 * for this content type to work.
 */
function ding_debt_debt_list_content_type_edit_form(&$form, &$form_state) {
  return $form;
}

