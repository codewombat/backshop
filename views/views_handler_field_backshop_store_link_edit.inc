<?php

/**
 * @file
 * Provides Backshop's store edit link field handler.
 */

/**
 * Field handler to present a link to edit a store.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_backshop_store_link_edit extends views_handler_field_backshop_store_link {

  /**
   * Renders the link.
   */
  function render_link($store, $values) {
    // Ensure the user has access to edit this store.
    if (!backshop_store_access('update', $store)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "admin/backshop/config/store/$store->store_id/edit";
    $this->options['alter']['query'] = backdrop_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Edit');
    return $text;
  }
}
