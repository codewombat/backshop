<?php

/**
 * @file
 * Provides Backshop's sort handler to sort by the default store.
 */

/**
 * Sort handler to sort the default store to the top of store queries.
 *
 * @ingroup views_sort_handlers
 */
class views_handler_sort_backshop_store_is_default extends views_handler_sort {
  function query() {
    $config = config('backshop.settings');
    $store_id = $config->get('default_store');
    $this->query->add_orderby(NULL, 'store_id = ' . $store_id, 'DESC');
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['order']['#access'] = FALSE;
    $form['order']['#default_value'] = 'DESC';
  }
}
