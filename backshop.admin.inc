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

/**
 * Form callback: builds the Backshop store add / edit form.
 *
 * @param BackshopStore|null $store
 *   (optional) The store being edited or NULL if a new store is being added.
 */
function backshop_store_form($form, &$form_state, BackshopStore $store = NULL) {
  global $user;

  // During initial form build, add the entity to the form state for use during
  // form building and processing. During a rebuild, use what is in the form
  // state.
  if (!isset($form_state['store'])) {
    if ($store === NULL) {
      $store = new BackshopStore(array('type' => 'online', 'uid' => $user->uid));
    }
    $form_state['store'] = $store;
  }
  else {
    $store = $form_state['store'];
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('The title of the store as it is known by customers.'),
    '#default_value' => $store->title,
  );

  field_attach_form('backshop_store', $store, $form, $form_state, $store->langcode);

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 50,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 0,
  );

  // Add a delete button if the store has an ID and can be deleted; otherwise
  // include a simple button for disabling the store.
  // @todo Add an #access parameter to these buttons based on delete access.
  // @todo Implement the delete and disable button submit handlers.
  if (!empty($store->store_id)) {
    if ($store->canDelete()) {
      $form['actions']['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#weight' => 10,
        '#suffix' => l(t('Cancel'), 'admin/backshop/config/store'),
      );
    }
    else {
      $form['actions']['disable'] = array(
        '#type' => 'submit',
        '#value' => t('Disable'),
        '#weight' => 10,
        '#suffix' => l(t('Cancel'), 'admin/backshop/config/store'),
      );
    }
  }
  else {
    $form['actions']['submit']['#suffix'] = l(t('Cancel'), 'admin/backshop/config/store');
  }

  return $form;
}

/**
 * Form callback: validates the store form.
 */
function backshop_store_form_validate($form, &$form_state) {
  // The form state contains the actual entity being edited, but it should not
  // update it with form values that have not yet been validated. We create here
  // a pseudo-entity to use during validation instead.
  $store = clone $form_state['store'];

  foreach ($form_state['values'] as $key => $value) {
    $store->{$key} = $value;
  }

  // Check to ensure the store has not been edited by someone else.
  if (isset($store->store_id) && (backshop_entity_last_changed('backshop_store', $store->store_id) > $store->changed)) {
    form_set_error('changed', t('This store has either been modified by another user or you have already submitted modifications using this form. Your changes cannot be saved, and the form must be reloaded.'));
  }

  // Validate the store owner.
  if (!empty($store->name) && !($account = user_load_by_name($store->name))) {
    // The use of empty() is mandatory in the context of usernames, as the empty
    // string denotes the anonymous user. If the anonymous user was specified,
    // we set the user ID to 0.
    form_set_error('name', t('The username %name does not exist.', array('%name' => $store->name)));
  }

  // Validate the created date field.
  if (!empty($store->date) && strtotime($store->date) === FALSE) {
    form_set_error('date', t('You have to specify a valid date.'));
  }

  field_attach_form_validate('backshop_store', $store, $form, $form_state);
}

/**
 * Form callback: submits the store form.
 */
function backshop_store_form_submit($form, &$form_state) {
  $store = $form_state['store'];

  // Remove unneeded values.
  form_state_values_clean($form_state);

  // Build and save the new or updated store.
  entity_form_submit_build_entity('backshop_store', $store, $form, $form_state);
  $result = $store->save();

  if ($result == SAVED_NEW) {
    backdrop_set_message(t('The store has been added.'));
  }
  else {
    backdrop_set_message(t('The store has been updated.'));
  }

  $form_state['redirect'] = 'admin/backshop/config/store/' . $store->id();
}
