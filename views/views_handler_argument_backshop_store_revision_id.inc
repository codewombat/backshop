<?php

/**
 * @file
 * Provides Backshop's store revision ID argument handler.
 */

/**
 * Argument handler to accept a store revision ID.
 */
class views_handler_argument_backshop_store_revision_id extends views_handler_argument_numeric {
  // No constructor is necessary.

  /**
   * Overrides views_handler_argument_numeric::title().
   * Gets the title of the revision.
   */
  function title_query() {
    $titles = array();

    $result = db_query("SELECT bs.title FROM {backshop_store_revision} bs WHERE bs.revision_id IN (:revision_ids)", array(':revision_ids' => $this->value));
    foreach ($result as $row) {
      $titles[] = check_plain($row->title);
    }
    return $titles;
  }
}
