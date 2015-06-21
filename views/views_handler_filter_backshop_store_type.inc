<?php

/**
 * @file
 * Provides Backshop's store type filter handler.
 */

/**
 * Filter by store type.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_backshop_store_type extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Store types');
      $bundles = backshop_entity_bundle_options('backshop_store');
      asort($bundles);
      $this->value_options = $bundles;
    }
  }
}
