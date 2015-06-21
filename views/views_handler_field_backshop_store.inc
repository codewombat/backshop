<?php

/**
 * @file
 * Provides Backshop's basic store field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a store.
 *
 * Definition terms:
 * - link_to_store default: boolean indicating whether or not this field have
 *   the checkbox "link to store" enabled by default.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_backshop_store extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);

    // Don't add the additional fields to groupby
    if (!empty($this->options['link_to_store'])) {
      $this->additional_fields['store_id'] = array('table' => 'backshop_store', 'field' => 'store_id');

      if (module_exists('translation')) {
        $this->additional_fields['langcode'] = array('table' => 'backshop_store', 'field' => 'langcode');
      }
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_store'] = array('default' => isset($this->definition['link_to_store default']) ? $this->definition['link_to_store default'] : FALSE, 'bool' => TRUE);
    return $options;
  }

  /**
   * Provide link to store option
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_store'] = array(
      '#title' => t('Link this field to the original store display'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_store']),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the store.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_store']) && !empty($this->additional_fields['store_id'])) {
      if ($data !== NULL && $data !== '') {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = "admin/backshop/config/store/" . $this->get_value($values, 'store_id');
        if (isset($this->aliases['langcode'])) {
          $languages = language_list();
          $language = $this->get_value($values, 'langcode');
          if (isset($languages[$language])) {
            $this->options['alter']['langcode'] = $languages[$language];
          }
          else {
            unset($this->options['alter']['langcode']);
          }
        }
      }
      else {
        $this->options['alter']['make_link'] = FALSE;
      }
    }
    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
