<?php

/**
 * Implements hook_default_message_type().
 */
function commerce_refund_default_message_type() {
  $items = array();
  $items['commerce_order_refunded'] = entity_import('message_type', '{
    "name" : "commerce_order_refunded",
    "description" : "Commerce Order: refunded",
    "argument_keys" : [],
    "argument" : [],
    "category" : "commerce_order_message",
    "data" : {
      "purge" : { "override" : 0, "enabled" : 0, "quota" : "", "days" : "" }
    },
    "language" : "",
    "arguments" : null,
    "message_text" : {
      "und" : [
        {
          "value" : "Order has been refunded.",
          "format" : null,
          "safe_value" : "Order has been refunded."
        },
        {
          "value" : "\\u003Cp\\u003EOrder has been refunded with \\u003Ca href=\\u0022[message:message-commerce-refund:view-url]\\u0022\\u003Einvoice [message:message-commerce-refund:order-number]\\u003C\\/a\\u003E.\\u003C\\/p\\u003E",
          "format" : "commerce_order_message",
          "safe_value" : "\\u003Cp\\u003EOrder has been refunded with \\u003Ca href=\\u0022[message:message-commerce-refund:view-url]\\u0022\\u003Einvoice [message:message-commerce-refund:order-number]\\u003C\\/a\\u003E.\\u003C\\/p\\u003E"
        }
      ]
    },
    "message_order_display_name" : {
      "und" : [ { "value" : "Order", "format" : null, "safe_value" : "Order" } ]
    },
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_default_message_type_alter().
 *
 * Use the LANGUAGE_NONE values for the other languages in multilingual sites.
 *
 * @see commerce_message_default_message_type_alter().
 */
function commerce_refund_default_message_type_alter(&$items) {
  if (module_exists('locale')) {
    $languages = locale_language_list();
    foreach ($languages as $langcode => $langname) {
      foreach ($items as $message_type => $item) {
        if ($item->module != 'commerce_refund') {
          continue;
        }
        if (isset($items[$message_type]->message_text[LANGUAGE_NONE])) {
          $items[$message_type]->message_text[$langcode] = $items[$message_type]->message_text[LANGUAGE_NONE];
        }
        if (isset($items[$message_type]->message_order_display_name[LANGUAGE_NONE])) {
          $items[$message_type]->message_order_display_name[$langcode] = $items[$message_type]->message_order_display_name[LANGUAGE_NONE];
        }
      }
    }
  }
}

/**
 * Implements hook_field_access().
 *
 * @see commerce_message_field_access().
 */
function commerce_refund_field_access($op, $field, $entity_type, $entity, $account) {
  if ($op == 'edit' && $field['field_name'] == 'message_commerce_refund') {
    return FALSE;
  }
}

/**
 * Implements hook_message_presave().
 *
 * Add token for the refund order summary.
 *
 * @see commerce_message_commerce_message_presave().
 */
function commerce_refund_message_presave($message) {
  if (!empty($message->mid) || $message->type != 'commerce_order_refunded') {
    return;
  }

  $message->arguments['!refund-summary'] = array(
    'callback' => 'commerce_refund_refund_summary',
    'pass message' => TRUE,
  );
  $message->arguments['!order-summary'] = array(
    'callback' => 'commerce_message_order_summary',
    'pass message' => TRUE,
  );
}

/**
 * Message callback; Show refund order summary.
 *
 * @see commerce_message_order_summary().
 */
function commerce_refund_refund_summary(Message $message) {
  $wrapper = entity_metadata_wrapper('message', $message);
  $view = views_get_view('commerce_cart_summary');
  $view->set_arguments(array($wrapper->message_commerce_refund->getIdentifier()));
  $view->hide_admin_links = TRUE;
  return $view->preview();
}

