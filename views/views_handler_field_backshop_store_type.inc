<?php

/**
 * @file
 * Provides Backshop's store type field handler.
 */

/**
 * Field handler to display the label of a store type.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_backshop_store_type extends views_handler_field_backshop_store {
  function option_definition() {
    $options = parent::option_definition();
    $options['machine_name'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  /**
   * Add the machine_name option for the display of the store type field.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['machine_name'] = array(
      '#title' => t('Display the store type machine name instead of its label.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['machine_name']),
    );
  }

  /**
    * Render the store type label unless the machine_name option was selected.
    */
  function render_name($data, $values) {
    if ($this->options['machine_name'] != 1 && $data !== NULL && $data !== '') {
      return t($this->sanitize_value(backshop_entity_bundle_label('backshop_store', $data)));
    }
    return $this->sanitize_value($data);
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->render_name($value, $values), $values);
  }
}
