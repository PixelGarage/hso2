<?php
/**
 * Created by PhpStorm.
 * User: ralph
 * Date: 30.07.17
 * Time: 18:55
 */


/**
 * Class Refactoring.
 *
 * A refactor base class used to implement a refactoring module.
 */
abstract class Refactoring {
  // Module name of module implementing refactoring class
  private $module_name = '';

  function __construct($modulename) {
    $this->module_name = $modulename;
  }

  /**
   * Executes all refactoring.
   */
  public function runAll() {
    // clear error message
    drupal_get_messages();

    // refactoring successfully completed
    return true;
  }

  /**
   * Resets all performed steps.
   */
  public function reset() {
    unset($_SESSION['class_refactoring']);
  }


  //
  // Refactoring step methods
  //
  /**
   * Returns the session data of this class.
   */
  private function &sessionData() {
    if (!isset($_SESSION['class_refactoring'])) {
      $_SESSION['class_refactoring'] = array();
    }
    return $_SESSION['class_refactoring'];
  }

  /**
   * Starts a given step.
   * Returns TRUE, if step has to be performed, false otherwise.
   */
  public function startStep($step_name) {
    // check if step is already performed
    if (isset($this->sessionData()[$step_name])) return false;

    //
    // reset step
    $this->resetStep($step_name);

    //
    // start step message
    drupal_set_message(t('Start step @step...', array('@step' => $step_name)), 'status');
    return true;
  }

  /**
   * Resets a given step.
   */
  public function endStep($step_name) {
    // step completed successfully
    $this->sessionData()[$step_name] = true;
    drupal_set_message(t('Step @step successfully completed', array('@step' => $step_name)), 'status');
    return true;
  }

  /**
   * Resets a given step.
   */
  public abstract function resetStep($step_name);


  private function stepExample() {
    try {
      // check if step has to be performed
      if (!$this->startStep('StepExample')) return true;

      // add steps here...

      return $this->endStep('StepExample');
    }
    catch (Exception $ex) {
      drupal_set_message(t('StepExample failed: @msg', array('@msg' => $ex->getMessage())), 'error');
      return false;
    }
  }



  //
  // Data structure helper functions
  //
  /**
   * Updates a bundle of an entity type.
   *
   * @param string $entity_type
   *    The entity type of the bundle to be updated, e.g. node, taxonomy_term, user...
   * @param string $bundle_path
   *    The path to the bundle file of entity type relative to the refactoring module.
   *    The file name has to equal the bundle name of the entity type, that has to be updated.
   *
   * @param array $delete_fields
   *    An array of field names to be deleted on the given bundle.
   */
  protected function updateEntityType($entity_type, $bundle_path, $delete_fields = array()) {
    // copy content type file to pixelstructure
    $bundle_name = pathinfo($bundle_path, PATHINFO_FILENAME);
    $source = 'this://' . $bundle_path;
    $dest = _pixel_structure_file_path($entity_type, $bundle_name);
    $this->copyFile($source, $dest);

    // update content type in db
    module_load_include('inc', 'pixel_structure', 'pixel_structure.admin');
    pixel_structure_entity_type_import($entity_type, $bundle_name, true);

    // delete field instances
    $this->deleteFieldInstances($entity_type, $bundle_name, $delete_fields);
  }

  /**
   * Adds or replaces a default view.
   *
   * @param $view_path string
   *    The path to the view file relative to the refactoring module. The file name
   *    has to equal the view name that is updated.
   */
  protected function updateView($view_path) {
    // copy view file to pixel_structure
    $view_name = pathinfo($view_path, PATHINFO_FILENAME);
    $source = 'this://' . $view_path;
    $dest = 'module://pixel_structure/views/' . $view_name . '.view';
    $this->copyFile($source, $dest);

    //  delete view in db
    if ($view = views_get_view($view_name)) {
      // Delete the view in db.
      views_delete_view($view);
    }
  }


  /**
   * Deletes the fields given by name, its field instances and data.
   *
   * @param $field_names
   *    An array of field names to be deleted.
   */
  protected function deleteFields($field_names) {
    // delete field instances
    foreach ($field_names as $field_name) {
      field_delete_field($field_name);
    }
    field_purge_batch(count($field_names) + 5);

    // make sure, the field_data and field_revision tables are dropped
    foreach ($field_names as $field_name) {
      $data_table = 'field_data_' . $field_name;
      $rev_table = 'field_revision_' . $field_name;
      db_drop_table($data_table);
      db_drop_table($rev_table);
    }
  }

  /**
   * Deletes the field instances given by name from the bundle of entity type.
   *
   * @param $entity_type
   * @param $bundle
   * @param $field_names
   *    An array of field names to be deleted in the bundle.
   */
  protected function deleteFieldInstances($entity_type, $bundle, $field_names) {
    // delete field instances
    foreach ($field_names as $field_name) {
      if ($instance = field_info_instance($entity_type, $field_name, $bundle)) {
        field_delete_instance($instance, false);
      }
    }
    field_purge_batch(count($field_names) + 5);
  }

  /**
   * Adds a term to the given vocabulary.
   *
   * @param $vocabulary_name
   * @param $term_name
   *
   * @return bool
   *    True, if term is successfully added or updated, false otherwise.
   */
  protected function updateTerm($vocabulary_name, $term_name, $new_name = null, $fields = array()) {
    //
    // check if term exists in vocabulary, update if any
    $terms = taxonomy_get_term_by_name($term_name, $vocabulary_name);
    if ($terms) {
      // update name of term
      foreach ($terms as $term) {
        if ($new_name) {
          $term->name = $new_name;
        }
        foreach($fields as $key => $value) {
          $term->$key = $value;
        }
        taxonomy_term_save($term);
      }
      return true;
    }

    //
    // add new term to vocabulary
    $vid = null;
    foreach(taxonomy_get_vocabularies() as $vocabulary) {
      if (strtolower($vocabulary->machine_name) == $vocabulary_name) {
        $vid = $vocabulary->vid;
        break;
      }
    }

    // create the new brand in brands vocabulary
    static $weight = 0;
    if (!empty($vid)) {
      $term = new stdClass();
      $term->name = $term_name;
      $term->vid = $vid;
      $term->weight = $weight++;
      foreach($fields as $key => $value) {
        $term->$key = $value;
      }
      taxonomy_term_save($term);
      return true;
    }
    return false;
  }

  /**
   * Deletes a term from the given vocabulary.
   *
   * @param $vocabulary_name
   * @param $term_name
   */
  protected function deleteTerm($vocabulary_name, $term_name) {
    $terms = taxonomy_get_term_by_name($term_name, $vocabulary_name);
    foreach ($terms as $term) {
      taxonomy_term_delete($term->tid);
    }
  }



  //
  // File helper functions
  //
  /**
   * Copies a file from source to destination.
   * Paths can be defined as relative path (to drupal root)  or with common schemas.
   *
   * Two additional schemes are supported:
   * - this://...  --> path within refactoring module
   * - module://[modulename]/... --> path within module 'modulename'
   * - theme://[themename]/... --> path within theme 'themename'
   */
  protected function copyFile($source, $destination) {
    $this->replaceScheme('this://', $source);
    $this->replaceScheme('module://', $source);
    $this->replaceScheme('theme://', $source);
    $this->replaceScheme('this://', $destination);
    $this->replaceScheme('module://', $destination);
    $this->replaceScheme('theme://', $destination);
    return file_unmanaged_copy($source, $destination, FILE_EXISTS_REPLACE);
  }


  //
  // Private functions
  //
  private function replaceScheme($scheme, &$path) {
    //
    // replace scheme in path with effective path
    if (substr($path, 0, strlen($scheme)) === $scheme) {
      //
      // replace module:// schema
      $rel_path = str_replace($scheme, '', $path);
      $path_elements = explode('/', $rel_path);
      $name = ($scheme == 'this://') ? $this->module_name : array_shift($path_elements);
      $type_path = implode('/', $path_elements);
      switch ($scheme) {
        case 'module://':
        case 'this://':
          $path = drupal_get_path('module', $name) . '/' . $type_path;
          break;
        case 'theme://':
          $path = drupal_get_path('theme', $name) . '/' . $type_path;
          break;
      }
    }
  }


} // end Refactoring class


