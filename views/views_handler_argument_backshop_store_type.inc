<?php

/**
 * @file
 * Provides Backshop's store type argument handler.
 */

/**
 * Argument handler to accept a store type.
 */
class views_handler_argument_backshop_store_type extends views_handler_argument_string {
  function construct() {
    parent::construct('type');
  }

  /**
   * Overrides views_handler_argument_string::summary_name().
   * Returns the store type label.
   */
  function summary_name($data) {
    return backshop_entity_bundle_label('backshop_store', $data->{$this->name_alias});
  }

  /**
   * Overrides views_handler_argument_string::title().
   * Returns the store type label.
   */
  function title() {
    return backshop_entity_bundle_label('backshop_store', $this->argument);
  }
}
