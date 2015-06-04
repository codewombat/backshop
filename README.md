Backshop
========

Backshop is an eCommerce project for Backdrop CMS that, much like Backdrop itself in relation to Drupal 7 and 8, sits
somewhere between Drupal Commerce 1 and 2. It offers a simpler framework than that provided by Drupal Commerce while
focusing on a more direct out of the box experience in line with the philosophy of Backdrop. However, it still takes
advantage of the standalone PHP libraries being developed to power key parts of Drupal Commerce 2.

As Backdrop describes itself as the comprehensive CMS for small to medium sized businesses and non-profits, Backshop
will focus on getting those users selling and collecting donations as quickly as possible. Included in the core project
itself are a handful of modules that integrate third party services to support eCommerce analytics, payment, and
subscription management, intentionally limiting the scope of features implemented directly in the Backdrop site.

Framework components
--------------------

Backdrop provides the same core systems used to power Drupal Commerce 1. Therefore, the eCommerce framework delivered
by this project is built around entities, fields, and the forms and user interfaces required to manage them. Consider
this list a roadmap for an initial release and refer to the codebase and commit log (for now) to track progress.

**Entity types:**

* Store
* Product
* Order
* Line item
* Payment

**Field types:**

* Price
* Address
* Entity reference

**Primary systems:**

* Shopping cart (including the Add to Cart form with attribute selection and product customizations)
* Checkout (including address book support for billing and shipping information)
* Payment (with out of the box integrations for select payment gateways)
* Discounts / Promotions
