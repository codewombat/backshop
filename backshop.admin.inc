<?php

/**
 * @file
 * Administrative page callbacks and form functions for Backshop.
 */


/**
 * Form callback: builds the Backshop store settings form.
 */
function backshop_store_settings_form($form, &$form_state) {
  $config = config('backshop.settings');

  // Build an options list of all available stores.
  $options = array();

  foreach (entity_load_multiple('backshop_store', FALSE) as $store_id => $store) {
    $options[$store_id] = $store->label() . ' ' . t('(Store ID: @store_id)', array('@store_id' => $store_id));
  }

  $form['default_store'] = array(
    '#type' => 'radios',
    '#title' => t('Default store'),
    '#options' => $options,
    '#default_value' => $config->get('default_store'),
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form callback: submits the Backshop store settings form.
 */
function backshop_store_settings_form_submit($form, &$form_state) {
  $config = config('backshop.settings');
  $config->set('default_store', $form_state['values']['default_store']);
  $config->save();

  drupal_set_message(t('Store settings saved.'));
}
