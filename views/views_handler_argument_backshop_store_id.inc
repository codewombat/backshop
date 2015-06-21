<?php

/**
 * @file
 * Provides Backshop's store ID argument handler.
 */

/**
 * Argument handler to accept a store ID.
 */
class views_handler_argument_backshop_store_id extends views_handler_argument_numeric {
  /**
   * Override the behavior of title(). Get the title of the store.
   */
  function title_query() {
    $titles = array();

    $result = db_query("SELECT bs.title FROM {backshop_store} bs WHERE bs.store_id IN (:store_ids)", array(':store_ids' => $this->value));
    foreach ($result as $row) {
      $titles[] = check_plain($row->title);
    }
    return $titles;
  }
}
