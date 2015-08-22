<?php

/**
 * @file
 * Provides Backshop's is / make default store field handler.
 */

/**
 * Field handler to indicate if a store is the default store and if not shows
 * a link to make it the default.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_backshop_store_is_default extends views_handler_field_entity {

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_make_default'] = array('default' => TRUE, 'bool' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_make_default'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link to the form make a store the default if it is not already.'),
      '#default_value' => $this->options['link_to_make_default'],
      '#weight' => -100,
    );
  }

  function render($values) {
    if ($entity = $this->get_value($values)) {
      $config = config('backshop.settings');

      if ($entity->id() == $config->get('default_store')) {
        return t('Default store');
      }
      else {
        return l(t('Make default'), 'admin/backshop/config/store/' . $entity->id() . '/make-default');
      }
    }
  }
}
