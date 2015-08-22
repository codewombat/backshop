<?php

/**
 * @file
 * Administrative page callbacks and form functions for Backshop.
 */


/**
 * Form callback: builds the Backshop default store confirmation form.
 *
 * @param $form
 *   The form array.
 * @param $form_state
 *   The form state array.
 * @param BackshopStore $store
 *   The store that should be made the new default.
 */
function backshop_store_make_default_form($form, &$form_state, $store) {
  $config = config('backshop.settings');

  /* @var BackshopStore $default_store */
  if ($default_store = entity_load('backshop_store', $config->get('default_store'))) {
    $form['current_default'] = array(
      '#type' => 'fieldset',
      '#title' => t('Current default: @label', array('@label' => $default_store->label())),
    );
    $form['current_default']['view'] = backshop_store_view($default_store);
  }

  $form['new_default'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update to: @label', array('@label' => $store->label())),
  );
  $form['new_default']['view'] = backshop_store_view($store);

  $form['new_default_store_id'] = array(
    '#type' => 'value',
    '#value' => $store->id(),
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update default store'),
  );

  // Prepare the cancel link options.
  if (isset($_GET['destination'])) {
    $path = urldecode($_GET['destination']);
  }
  else {
    $path = 'admin/backshop/config/store';
  }
  $options = backdrop_parse_url($path);

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => $options['path'],
    '#options' => $options,
    '#weight' => 20,
  );

  return $form;
}

/**
 * Form callback: submits the Backshop default store confirmation form.
 */
function backshop_store_make_default_form_submit($form, &$form_state) {
  $store = entity_load('backshop_store', $form_state['values']['new_default_store_id']);

  $config = config('backshop.settings');
  $config->set('default_store', $store->id());
  $config->save();

  drupal_set_message(t('%label is now the default store for this site.', array('%label' => $store->label())));

  if (!isset($_GET['destination'])) {
    $form_state['redirect'] = $form['actions']['cancel']['#href'];
  }
}
