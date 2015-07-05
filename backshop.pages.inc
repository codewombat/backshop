<?php

/**
 * @page
 * General page callbacks and form functions for Backshop.
 */


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
      $store = new BackshopStore(array(
        'type' => 'online',
        'uid' => $user->uid,
        'status' => TRUE,
      ));
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

  $form['status'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#description' => t('Disabled stores cannot be viewed or purchased from.'),
    '#options' => array(
      '1' => t('Active'),
      '0' => t('Disabled'),
    ),
    '#default_value' => $store->status,
    '#required' => TRUE,
    '#weight' => 35,
  );

  dpm($store);

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
  if (!empty($store->store_id)) {
    if ($store->canDelete()) {
      $form['actions']['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#limit_validation_errors' => array(),
        '#submit' => array('backshop_store_form_submit_delete'),
        '#weight' => 10,
      );
    }
    else {
      $form['actions']['disable'] = array(
        '#type' => 'submit',
        '#value' => t('Cannot delete (disable instead)'),
        '#disabled' => TRUE,
        '#weight' => 10,
      );
    }
  }

  // Prepare the cancel link options.
  if (isset($_GET['destination'])) {
    $path = urldecode($_GET['destination']);
  }
  elseif (isset($_SERVER['HTTP_REFERER'])) {
    $path = urldecode($_SERVER['HTTP_REFERER']);
  }
  elseif (isset($store->store_id)) {
    $path = 'store/' . $store->store_id;
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

  $form_state['redirect'] = 'store/' . $store->id();
}

/**
 * Form callback: submits the store delete button.
 */
function backshop_store_form_submit_delete($form, &$form_state) {
  $form_state['redirect'] = 'store/' . $form_state['store']->id() . '/delete';
}

/**
 * Form callback: builds the store delete form.
 */
function backshop_store_delete_form($form, &$form_state, $store) {
  $form_state['store'] = $store;

  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $store->label())),
    'store/' . $store->id(),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form callback: submits the store delete form.
 */
function backshop_store_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $store = backshop_store_load($form_state['store']->id());
    entity_delete_multiple('backshop_store', array($store->id()));

    watchdog('backshop', '@type: deleted %title.', array('@type' => $store->bundleLabel(), '%title' => $store->label()));
    backdrop_set_message(t('@type %title has been deleted.', array('@type' => $store->bundleLabel(), '%title' => $store->label())));
  }

  $form_state['redirect'] = '<front>';
}
