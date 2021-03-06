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
  // a group defined will go into this field by default.
  $data['backshop_store']['table']['group'] = t('Backshop Store');
  $data['backshop_store']['table']['entity type'] = 'backshop_store';

  // Advertise this table as a possible base table
  $data['backshop_store']['table']['base'] = array(
    'field' => 'store_id',
    'title' => t('Backshop Stores'),
    'weight' => -10,
    'access query tag' => 'backshop_entity_access',
    'defaults' => array(
      'field' => 'title',
    ),
  );

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
      'help' => t('Relate the store to the user who owns it.'),
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

  $data['views_entity_backshop_store']['is_default'] = array(
    'title' => t('Default store'),
    'help' => t('Sort the default store to the top of the query results.'),
    'field' => array(
      'title' => t('Default store'),
      'help' => t('Indicates if the store is the default store for the site and offers a link to make it so if not.'),
      'handler' => 'views_handler_field_backshop_store_is_default',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_backshop_store_is_default',
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

  // Define the base group of the backshop_store_revision table. Fields that
  // don't have a group defined will go into this field by default.
  $data['backshop_store_revision']['table']['group']  = t('Backshop Store revision');
  $data['backshop_store_revision']['table']['entity type'] = 'backshop_store';

  // Advertise this table as a possible base table.
  $data['backshop_store_revision']['table']['base'] = array(
    'field' => 'revision_id',
    'title' => t('Backshop Store revisions'),
    'help' => t('Store revision is a history of changes to stores.'),
    'defaults' => array(
      'field' => 'title',
    ),
  );

  // For other base tables, explain how we join.
  $data['backshop_store_revision']['table']['join'] = array(
    // Directly links to the backshop_store table.
    'backshop_store' => array(
      'left_field' => 'revision_id',
      'field' => 'revision_id',
    ),
  );

  $data['backshop_store_revision']['table']['default_relationship'] = array(
    'backshop_store' => array(
      'table' => 'backshop_store',
      'field' => 'store_id',
    ),
  );

  // Define the uid field for store revisions.
  $data['backshop_store_revision']['uid'] = array(
    'title' => t('Creator'),
    'help' => t('Relate a store revision to the user who created the revision.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'label' => t('revision user'),
    ),
  );

  // Define the store ID field for store revisions.
  $data['backshop_store_revision']['store_id'] = array(
    'title' => t('Store ID'),
    'help' => t('The store ID of the store revision.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_backshop_store_id',
      'click sortable' => TRUE,
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'backshop_store',
      'base field' => 'store_id',
      'title' => t('Store'),
      'label' => t('Get the actual store from a store revision.'),
    ),
  );

  // Define the revision ID field for store revisions.
  $data['backshop_store_revision']['revision_id'] = array(
    'title' => t('Revision ID'),
    'help' => t('The revision ID of the store revision.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_backshop_store_revision_id',
      'click sortable' => TRUE,
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'backshop_store',
      'base field' => 'revision_id',
      'title' => t('Store'),
      'label' => t('Get the actual store from a store revision.'),
    ),
  );

  // Define the store revision title field.
  $data['backshop_store_revision']['title'] = array(
    'title' => t('Title'),
    'help' => t('The store revision title.'),
    'field' => array(
      'field' => 'title',
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
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

  // Define the store revision log field.
  $data['backshop_store_revision']['log'] = array(
    'title' => t('Log message'),
    'help' => t('The log message entered when the revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  // Define the store revision changed timestamp field.
  $data['backshop_store_revision']['timestamp'] = array(
    'title' => t('Creation date'),
    'help' => t('The date the revision was created.'),
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

  return $data;
}
