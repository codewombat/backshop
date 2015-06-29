<?php

/**
 * @file
 * Provides Backshop's store delete link field handler.
 */

/**
 * Field handler to present a link to delete a store.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_backshop_store_link_delete extends views_handler_field_backshop_store_link {

  /**
   * Renders the link.
   */
  function render_link($store, $values) {
    // Ensure user has access to delete this store.
    if (!backshop_store_access('delete', $store)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "store/$store->store_id/delete";
    $this->options['alter']['query'] = backdrop_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Delete');
    return $text;
  }
}
