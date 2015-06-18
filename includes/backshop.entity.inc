<?php

/**
 * @file
 * Entity class and storage controller for versioned entities.
 */


/**
 * Defines the Backshop store entity class.
 */
class BackshopStore extends Entity {

  /**
   * The store ID.
   *
   * @var integer
   */
  public $store_id;

  /**
   * The store revision ID.
   *
   * @var integer
   */
  public $revision_id;

  /**
   * The store type (bundle).
   *
   * @var string
   */
  public $type;

  /**
   * The store language code.
   *
   * @var string
   */
  public $langcode = LANGUAGE_NONE;

  /**
   * The store title.
   *
   * @var string
   */
  public $title;

  /**
   * The store owner's user ID.
   *
   * @var integer
   */
  public $uid;

  /**
   * The store's status indicator.
   *
   * A disabled store cannot be actively purchased from, though references to
   * the store may still be established and maintained. The value is either
   * BACKSHOP_ENTITY_ACTIVE or BACKSHOP_ENTITY_DISABLED.
   *
   * @var integer
   */
  public $status;

  /**
   * The store creation timestamp.
   *
   * @var integer
   */
  public $created;

  /**
   * The store modification timestamp.
   *
   * @var integer
   */
  public $changed;

  /**
   * The current revision creation timestamp.
   *
   * @var integer
   */
  public $revision_timestamp;

  /**
   * The current revision author's user ID.
   *
   * @var integer
   */
  public $revision_uid;

  /**
   * During the build content routine, this variable contains the renderable
   * array that represents the content of this entity.
   */
  public $content;

  /**
   * Create a new Node instance.
   */
  function __construct(array $values = array()) {
    parent::__construct($values);

    // Set the created time to now.
    if (empty($this->created)) {
      $this->created = REQUEST_TIME;
    }
  }

  /**
   * Implements EntityInterface::id().
   */
  public function id() {
    return isset($this->store_id) ? $this->store_id : NULL;
  }

  /**
   * Implements EntityInterface::entityType().
   */
  public function entityType() {
    return 'backshop_store';
  }

  /**
   * Implements EntityInterface::bundle().
   */
  public function bundle() {
    return $this->type;
  }

  /**
   * Implements EntityInterface::label().
   */
  public function label() {
    return $this->title;
  }

  /**
   * Implements EntityInterface::uri().
   */
  public function uri() {
    return NULL;
  }

  /**
   * Determines whether or not the current store can be deleted.
   *
   * Any store that is referenced by another entity cannot be deleted. They
   * should be disabled instead.
   */
  public function canDelete() {
    return FALSE;
  }

  /**
   * Builds a structured array representing the entity's content.
   *
   * @param string $view_mode
   *   (optional) The view mode to build the array for, e.g. 'full', 'teaser',
   *   'summary', etc. Defaults to 'full'.
   * @param string $langcode
   *   (optional) A language code to use for generating the content. Defaults to
   *   the global content language of the current request.
   * @param array $default_content
   *   (optional) The default array to start building the content from.
   *
   * @return array
   *   The renderable array.
   */
  public function buildContent($view_mode = 'full', $langcode = NULL, $default_content = array()) {
    // Remove previously built content, if exists.
    $this->content = $default_content;

    // Default to the global language if necessary.
    if (!isset($langcode)) {
      $langcode = $GLOBALS['language_content']->langcode;
    }

    // Allow modules to change the view mode.
    $view_mode = key(entity_view_mode_prepare($this->entityType(), array($this->id() => $this), $view_mode, $langcode));

    // Build field content arrays. In the case of a multiple view, the entity
    // controller's view method already ran the 'prepare_view' step. An internal
    // flag prevents the operation from running twice.
    $entity_info = entity_get_info($this->entityType());

    // Prepare the field and entity data for viewing.
    if (!empty($entity_info['fieldable'])) {
      field_attach_prepare_view($this->entityType(), array($this->id() => $this), $view_mode, $langcode);
    }

    entity_prepare_view($this->entityType(), array($this->id() => $this), $langcode);

    // Add field content to the array if the entity is fieldable.
    if (!empty($entity_info['fieldable'])) {
      $this->content += field_attach_view($this->entityType(), $this, $view_mode, $langcode);
    }

    // Invoke hook_ENTITY_TYPE_view() to allow modules to add their additions.
    module_invoke_all($this->entityType() . '_view', $this, $view_mode, $langcode);

    // Invoke the more generic hook_entity_view() to allow the same.
    module_invoke_all('entity_view', $this, $this->entityType(), $view_mode, $langcode);

    // Make sure the current view mode is stored if no module has already
    // populated the related key.
    $this->content += array('#view_mode' => $view_mode);

    // Remove the build array information from the entity and return it.
    $build = $this->content;
    unset($this->content);

    return $build;
  }
}

/**
 * Storage controller class for versioned entities.
 *
 * This extends the EntityDatabaseStorageController class, adding required
 * special handling for managing entity revisions.
 */
class BackshopVersionedEntityStorageController extends EntityDatabaseStorageController {

  /**
   * Overrides EntityDatabaseStorageController::delete().
   */
  public function delete($ids) {
    $entities = $ids ? $this->load($ids) : FALSE;

    // If no IDs or invalid IDs were passed, do nothing.
    if (!$entities) {
      return;
    }

    $transaction = db_transaction();

    try {
      $this->preDelete($entities);

      foreach ($entities as $id => $entity) {
        $this->invokeHook('predelete', $entity);
      }

      $ids = array_keys($entities);

      db_delete($this->entityInfo['base table'])
        ->condition($this->idKey, $ids, 'IN')
        ->execute();

      if ($this->revisionKey) {
        db_delete($this->revisionTable)
          ->condition($this->idKey, $ids, 'IN')
          ->execute();
      }

      // Reset the cache as soon as the changes have been applied.
      $this->resetCache($ids);

      $this->postDelete($entities);

      foreach ($entities as $id => $entity) {
        $this->invokeHook('delete', $entity);
      }

      // Ignore the slave server temporarily.
      db_ignore_slave();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception($this->entityType, $e);
      throw new EntityStorageException($e->getMessage(), (int) $e->getCode(), $e);
    }
  }

  /**
   * Overrides EntityDatabaseStorageController::save().
   */
  public function save(EntityInterface $entity) {
    $transaction = db_transaction();

    try {
      // Load the stored entity, if any.
      if (!$entity->isNew() && !isset($entity->original)) {
        $entity->original = entity_load_unchanged($this->entityType, $entity->id());
      }

      $this->preSave($entity);
      $this->invokeHook('presave', $entity);

      if ($entity->isNew()) {
        $op = 'insert';
        $return = backdrop_write_record($this->entityInfo['base table'], $entity);
        unset($entity->is_new);
      }
      else {
        $op = 'update';
        $return = backdrop_write_record($this->entityInfo['base table'], $entity, $this->idKey);
      }

      if ($this->revisionKey) {
        $this->saveRevision($entity);
      }

      // Reset general caches, but keep caches specific to certain entities.
      $this->resetCache($op == 'update' ? array($entity->{$this->idKey}): array());

      $this->postSave($entity, $op == 'update');
      $this->invokeHook($op, $entity);

      // Ignore slave server temporarily.
      db_ignore_slave();
      unset($entity->original);

      return $return;
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception($this->entityType, $e);
      throw new EntityStorageException($e->getMessage(), (int) $e->getCode(), $e);
    }
  }

  /**
   * Saves a revision.
   *
   * @param EntityInterface $entity
   *   The entity being saved.
   */
  protected function saveRevision(EntityInterface $entity) {
    $revision = clone $entity;
    $revision->uid = $entity->revision_uid;
    $revision->timestamp = $entity->revision_timestamp;

    if (empty($entity->{$this->revisionKey}) || !empty($entity->revision)) {
      backdrop_write_record($this->revisionTable, $revision);
      db_update($this->entityInfo['base table'])
        ->fields(array($this->revisionKey => $revision->{$this->revisionKey}))
        ->condition($this->idKey, $entity->{$this->idKey})
        ->execute();
    }
    else {
      backdrop_write_record($this->revisionTable, $revision, $this->revisionKey);
    }

    // Make sure to update the new revision key for the entity.
    $entity->{$this->revisionKey} = $revision->{$this->revisionKey};
  }

  /**
   * Overrides DefaultEntityController::buildQuery().
   */
  protected function buildQuery($ids, $conditions = array(), $revision_id = FALSE) {
    // Ensure that uid is taken from the entity's base table, alias timestamp
    // to revision_timestamp, and add revision_uid.
    $query = parent::buildQuery($ids, $conditions, $revision_id);
    $fields =& $query->getFields();
    unset($fields['timestamp']);
    $query->addField('revision', 'timestamp', 'revision_timestamp');
    $fields['uid']['table'] = 'base';
    $query->addField('revision', 'uid', 'revision_uid');
    return $query;
  }

  /**
   * Overrides EntityDatabaseStorageController::preSave().
   */
  protected function preSave(EntityInterface $entity) {
    // Before saving the entity, set changed and revision times.
    $entity->changed = REQUEST_TIME;

    if ($this->revisionKey && !empty($entity->revision)) {
      $entity->revision_timestamp = REQUEST_TIME;

      if (!isset($entity->revision_uid)) {
        $entity->revision_uid = $GLOBALS['user']->uid;
      }
    }

    // When inserting either a new entity or a new revision, $entity->log must
    // be set because the log column is a text column and therefore cannot have
    // a default value. However, it might not be set at this point (for example,
    // if the user submitting an entity form does not have permission to create
    // revisions), so we ensure that it is at least an empty string.
    if ($entity->isNew() || !empty($entity->revision)) {
      if (!isset($entity->log)) {
        $entity->log = '';
      }
    }
    elseif (!isset($entity->log) || $entity->log === '') {
      // If we are updating an existing entity without adding a new revision,
      // we need to make sure $entity->log is unset whenever it is empty. As
      // long as $entity->log is unset, backdrop_write_record() will not
      // attempt to update the existing database column when re-saving the
      // revision; therefore, this code allows us to avoid clobbering an
      // existing log entry with an empty one.
      unset($entity->log);
    }

    // When saving a new revision, unset any existing $entity->vid to ensure
    // that a new revision will actually be created, storing the old revision
    // ID in a temporary property for use in other hook functions.
    if (!$entity->isNew() && !empty($entity->revision) && $entity->{$this->revisionKey}) {
      $entity->old_revision_id = $entity->{$this->revisionKey};
      $entity->{$this->revisionKey} = NULL;
    }
  }

  /**
   * Generate an array for rendering the given entities.
   *
   * @todo Add support for a page parameter as in the Drupal 7 Entity API.
   *
   * @param array $entities
   *   An array of entities implementing EntityInterface to render.
   * @param string $view_mode
   *   (optional) The view mode to build the array for, e.g. 'full', 'teaser',
   *   'summary', etc. Defaults to the first view mode found in the entity info.
   * @param string $langcode
   *   (optional) A language code to use for generating the content. Defaults to
   *   the global content language of the current request.
   *
   * @return array
   *   The renderable array.
   */
  public function view($entities, $view_mode = '', $langcode = NULL) {
    // Create a new entities array keyed by entity ID.
    $rekeyed_entities = array();

    foreach ($entities as $key => $entity) {
      $rekeyed_entities[$entity->id()] = $entity;
    }

    $entities = $rekeyed_entities;

    // If a valid view mode is not specified, use the first one available.
    if (!isset($this->entityInfo['view modes'][$view_mode])) {
      reset($this->entityInfo['view modes']);
      $view_mode = key($this->entityInfo['view modes']);
    }

    if (!empty($this->entityInfo['fieldable'])) {
      field_attach_prepare_view($this->entityType, $entities, $view_mode);
    }

    entity_prepare_view($this->entityType, $entities);
    $langcode = isset($langcode) ? $langcode : $GLOBALS['language_content']->language;
    $view = array();

    // Build the content array for each entity passed in.
    foreach ($entities as $key => $entity) {
      $build = $entity->buildContent($view_mode, $langcode);

      // Add default properties to the array to ensure the content is passed
      // through the theme layer.
      $build += array(
        '#theme' => 'entity',
        '#entity_type' => $this->entityType,
        '#entity' => $entity,
        '#view_mode' => $view_mode,
        '#language' => $langcode,
      );

      // Allow modules to modify the build array.
      backdrop_alter(array($this->entityType . '_view', 'entity_view'), $build, $this->entityType);
      $view[$this->entityType][$key] = $build;
    }

    return $view;
  }
}

