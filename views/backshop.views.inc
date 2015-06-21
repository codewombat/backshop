<?php

/**
 * @file
 * Provide Views data definitions for backshop.module.
 *
 * @ingroup views_module_handlers
 */

/**
 * Implements hook_views_data().
 */
function backshop_views_data() {
  // Define the base group of the backshop_store table. Fields that don't have
  // have a group defined will go into this field by default.
  $data['backshop_store']['table']['group'] = t('Backshop Store');

  // Advertise this table as a possible base table
  $data['backshop_store']['table']['base'] = array(
    'field' => 'store_id',
    'title' => t('Backshop Store'),
    'weight' => -10,
    'access query tag' => 'backshop_store_access',
    'defaults' => array(
      'field' => 'title',
    ),
  );
  $data['backshop_store']['table']['entity type'] = 'backshop_store';

  $data['backshop_store']['table']['default_relationship'] = array(
    'backshop_store_revision' => array(
      'table' => 'backshop_store_revision',
      'field' => 'revision_id',
    ),
  );

  // Define the store ID field.
  $data['backshop_store']['store_id'] = array(
    'title' => t('Store ID'),
    'help' => t('The serial numeric store ID.'),
    'field' => array(
      'handler' => 'views_handler_field_backshop_store',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_backshop_store_id',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'store_id',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Define the store title field.
  $data['backshop_store']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the store.'),
    'field' => array(
      'handler' => 'views_handler_field_backshop_store',
      'click sortable' => TRUE,
      'link_to_store default' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Define the created date field.
  $data['backshop_store']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the store was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Define the changed date field.
  $data['backshop_store']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the store was last updated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Define the store type field.
  $data['backshop_store']['type'] = array(
    'title' => t('Type'),
    'help' => t('The store type (e.g. Online store).'),
    'field' => array(
      'handler' => 'views_handler_field_backshop_store_type',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_backshop_store_type',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_backshop_store_type',
    ),
  );

  // Define the status field.
  $data['backshop_store']['status'] = array(
    'title' => t('Status'),
    'help' => t('Whether or not the store is active.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'active-disabled' => array(t('Active'), t('Disabled')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Active?'),
      'type' => 'yes-no',
      'use equal' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Define the uid field.
  $data['backshop_store']['uid'] = array(
    'title' => t('Owner user ID'),
    'help' => t('The user who owns the store. If you need more fields than the uid add the Store: Owner relationship'),
    'relationship' => array(
      'title' => t('Owner'),
      'help' => t('Relate content to the user who owns it.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('owner'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
  );

  // Define a bulk operations checkbox field.
  /**
   * @todo Re-enable when Backshop actually defines relevant actions.
  $data['backshop_store']['bulk_form'] = array(
    'title' => t('Bulk operations'),
    'help' => t('Provides a checkbox on each row and a form for executing bulk actions on stores.'),
    'real field' => 'store_id',
    'field' => array(
      'handler' => 'views_handler_field_bulk_form',
      'click sortable' => FALSE,
    ),
  );
   */

  // Define some fields based upon views_handler_field_entity in the entity
  // table so they can be re-used with other query backends.
  // @see views_handler_field_entity
  $data['views_entity_backshop_store']['table']['group'] = t('Backshop Store');

  $data['views_entity_backshop_store']['view_store'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the store.'),
      'handler' => 'views_handler_field_backshop_store_link',
    ),
  );

  $data['views_entity_backshop_store']['edit_store'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the store.'),
      'handler' => 'views_handler_field_backshop_store_link_edit',
    ),
  );

  $data['views_entity_backshop_store']['delete_store'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the store.'),
      'handler' => 'views_handler_field_backshop_store_link_delete',
    ),
  );

  /**
   * @todo Implement a path handler when there is a Backshop entity type that
   * supports path aliases.
  $data['backshop_store']['path'] = array(
    'field' => array(
      'title' => t('Path'),
      'help' => t('The aliased path to this store.'),
      'handler' => 'views_handler_field_backshop_store_path',
    ),
  );
   */

  return $data;
}
