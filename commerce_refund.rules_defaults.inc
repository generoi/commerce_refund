<?php
/**
 * @file
 * commerce_refund.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_refund_default_rules_configuration() {
  $items = array();
  $items['commerce_refund_create_refund_order'] = entity_import('rules_config', '{
    "commerce_refund_create_refund_order" : {
      "LABEL" : "Refund an order",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [
        "rules",
        "commerce_refund",
        "commerce_order",
        "commerce_line_item",
        "entity"
      ],
      "ON" : { "commerce_order_update" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "commerce-order:status" ], "value" : "refund" } },
        { "entity_is_of_bundle" : {
            "entity" : [ "commerce-order" ],
            "type" : "commerce_order",
            "bundle" : { "value" : { "commerce_order" : "commerce_order" } }
          }
        }
      ],
      "DO" : [
        { "commerce_refund_create_refund" : {
            "USING" : { "commerce_order" : [ "commerce_order" ] },
            "PROVIDE" : { "reversed_order" : { "reversed_order" : "Reversed order" } }
          }
        },
        { "commerce_order_update_status" : { "commerce_order" : [ "reversed-order" ], "order_status" : "refund_pending" } },
        { "data_set" : {
            "data" : [ "reversed-order:commerce-refund-order-field:0" ],
            "value" : [ "commerce-order" ]
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "reversed-order:commerce-line-items" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "commerce_line_item_unit_price_multiply" : {
                  "commerce_line_item" : [ "list-item" ],
                  "amount" : "-1",
                  "component_name" : "base_price",
                  "round_mode" : "1"
                }
              },
              { "entity_save" : { "data" : [ "list-item" ], "immediate" : 1 } }
            ]
          }
        },
        { "entity_save" : { "data" : [ "reversed-order" ] } },
        { "list_add" : {
            "list" : [ "commerce-order:commerce-refund-order-field" ],
            "item" : [ "reversed-order" ],
            "unique" : 1
          }
        },
        { "commerce_order_update_status" : { "commerce_order" : [ "commerce-order" ], "order_status" : "refund" } },
        { "entity_save" : { "data" : [ "commerce-order" ], "immediate" : 1 } },
        { "entity_create" : {
            "USING" : {
              "type" : "message",
              "param_type" : "commerce_order_refunded",
              "param_user" : [ "commerce-order:owner" ]
            },
            "PROVIDE" : { "entity_created" : { "message_created" : "Created message" } }
          }
        },
        { "data_set" : {
            "data" : [ "message-created:message-commerce-order" ],
            "value" : [ "commerce-order" ]
          }
        },
        { "data_set" : {
            "data" : [ "message-created:message-commerce-refund" ],
            "value" : [ "reversed-order" ]
          }
        },
        { "entity_save" : { "data" : [ "message-created" ] } },
        { "redirect" : { "url" : "[reversed-order:edit-url]"  }  }
      ]
    }
  }');

  $items['commerce_refund_refund_complete'] = entity_import('rules_config', '{
    "commerce_refund_refund_complete" : {
      "LABEL" : "E-mail refund invoice to customer",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "message_notify", "entity" ],
      "ON" : { "commerce_order_presave" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "commerce-order:status" ], "value" : "refund_completed" } },
        { "entity_has_field" : {
            "entity" : [ "commerce-order" ],
            "field" : "commerce_refund_order_field"
          }
        },
        { "NOT data_is" : {
            "data" : [ "commerce-order-unchanged:status" ],
            "value" : "refund_completed"
          }
        }
      ],
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "message",
              "param_type" : "commerce_order_refunded",
              "param_user" : [ "commerce-order:owner" ]
            },
            "PROVIDE" : { "entity_created" : { "entity_created" : "Created entity" } }
          }
        },
        { "data_set" : {
            "data" : [ "entity-created:message-commerce-refund" ],
            "value" : [ "commerce-order" ]
          }
        },
        { "data_set" : {
            "data" : [ "entity-created:message-commerce-order" ],
            "value" : [ "commerce-order:commerce-refund-order-field:0" ]
          }
        },
        { "entity_save" : { "data" : [ "entity-created" ], "immediate" : 1 } },
        { "message_notify_process" : {
            "message" : [ "entity-created" ],
            "save_on_fail" : 0,
            "save_on_success" : 0
          }
        }
      ]
    }
  }');
  return $items;
}
