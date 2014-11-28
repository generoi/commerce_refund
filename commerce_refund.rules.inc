<?php

/**
 * @file commerce_refund.module
 * Rules integratoin file.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_refund_rules_action_info() {
  return array(
    'commerce_refund_create_refund' => array(
      'label' => t('Create and save a refund order'),
      'parameter' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('Source order'),
        ),
      ),
      'group' => t('Commerce Refund'),
      'base' => 'commerce_refund_action_order_clone',
      'provides' => array(
        'reversed_order' => array(
          'label' => 'Reversed order',
          'type' => 'commerce_order',
          'bundle' => 'commerce_order',
          'save' => TRUE,
        ),
      ),
    ),
  );
}
