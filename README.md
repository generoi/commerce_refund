Commerce Refund
===============

> Logic and bundled defaults for issuing order refunds.

Description
-----------

- Creates a new `refund` state and two statueses for cloned orders which are
  acting as refund orders.
- Creates a new `refund` status on the complete state for marking and trigger
  refunds on customer orders.
- Defines a custom message type for refund orders, that can be used to email
  refund invoices to customers.
- Defines a _Create and save a refund order_ rule action which clones an order
  with the `Refund: pending` status.
- Bundles a default rule for creating refunds, negating their product prices
  etc.
- Bundles a default rule for emailing the refund invoice.
- Adds an entityreference field to _Commerce Orders_ which references orders
  and refunds to eachother.
- Adds a `!refund-summary` token to the new message type.

Installation
------------
1. Install and enable the module
2. Configure the default rules if necessary. The _E-mail refund invoice to
   customer_ could for example set the invoice id.

Usage
-----
1. Locates the order to refund and sets its status to `Completed: Refunded user
   order`.
2. Locate the refund order (the refund-order id was logged in the history of
   the original order) and edit the products as you please. Eg. changing
   quantities, adding static products etc.
3. Once the refund order is setup and you want to email it to the customer -
   change the status to `Refund: Completed refund order`.

Limitations
-----------
- Taxes, discounts, shipping etc are not calculated.
- This is basically just a simple order clone workflow to ease refund
  management.
- It is up to the administrator to make sure refund calculations are correct,
  you are allowed to do multiple refunds of the same order and the module will
  not check quantities.
