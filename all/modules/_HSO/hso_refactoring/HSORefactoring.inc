<?php
/**
 * Created by PhpStorm.
 * User: ralph
 * Date: 27.07.17
 * Time: 17:38
 */


class HSORefactoring extends Refactoring {

  //
  // define location mapping nid_old => nid_new
  private $location_mapping = array(
    15128 => 6687,  // KMZ Rapperswil -> HBS Rapperswil
    //6687 => 5942,   // HBS Rapperswil -> Rapperswil
    32727 => 6685,  // HMZ Baar -> Baar
    8803 => 579,    // BVS St.Gallen -> St.Gallen
  );


  /**
   * Executes all refactoring.
   */
  public function runAll() {
    // call parent function
    parent::runAll();

    // enable new modules
    if (!module_exists('views_bulk_operations')) {
      module_enable(array('views_bulk_operations'), true);
    }

    //
    // run all refactoring step by step. Each step has to succeed
    // to continue refactoring
    if (!$this->enhanceViewCourses()) return 'EnhanceViewCourses';
    if (!$this->contactCleanup()) return 'ContactCleanup';
    if (!$this->courseTimesCleanup()) return 'CourseTimesCleanup';
    if (!$this->locationAndBrandsCleanup()) return 'LocationAndBrandsCleanup';
    if (!$this->bannerExtension()) return 'BannerExtension';
    if (!$this->viewsUpdate()) return 'ViewsUpdate';

    // refactoring successfully completed
    return TRUE;
  }

  /**
   * Resets a given step.
   */
  public function resetStep($step_name) {
    // reset contact cleanup, if not finished
    if ($step_name == 'ContactCleanup') {
      // make sure that newly added fields are completely deleted (also DB tables)
      $this->deleteFields(array('field_courses', 'field_locations'));
    }
    else if ($step_name == 'CourseTimesCleanup') {
      // make sure that newly added fields are completely deleted (also DB tables)
      $this->deleteFields(array('field_study_variants', 'field_master_nid'));
    }
    else if ($step_name == 'BannerExtension') {
      // make sure that newly added fields are completely deleted (also DB tables)
      $this->deleteFields(array('field_banner_link', 'field_interval'));
    }

  }


  //
  // Steps implementation
  //
  /**
   * Enhance view courses with course references.
   */
  public function enhanceViewCourses() {
    try {
      // check if step has to be performed
      if (!$this->startStep('EnhanceViewCourses')) return true;

      // add additionalviews here...
      $this->updateView('views/courses.view');

      // clear views cache
      if (module_exists('views')) {
        views_invalidate_cache();
      }

      return $this->endStep('EnhanceViewCourses');
    }
    catch (Exception $ex) {
      drupal_set_message(t('EnhanceViewCourses failed: @msg', array('@msg' => $ex->getMessage())), 'error');
      return false;
    }
  }

  /**
   * Cleanup all contacts
   */
  public function contactCleanup() {
    try {
      // check if step has to be performed
      if (!$this->startStep('ContactCleanup')) return true;

      // perform contact cleanup
      $this->updateEntityType('node', 'nodes/contact.node', array(
        'field_twitter_url',
        'field_google_url',
        'field_xing_url',
        'field_linkedin_url',
      ));
      $this->combineContacts();
      $this->deleteFieldInstances('node', 'contact', array(
        'field_location',
        'field_course',
        'field_internal_id',
      ));
      $this->updateLocationReferencesOnBundle('contact', true);
      $this->updateView('views/contacts.view');

      return $this->endStep('ContactCleanup');
    }
    catch (Exception $ex) {
      drupal_set_message(t('ContactCleanup failed: @msg', array('@msg' => $ex->getMessage())), 'error');
      return false;
    }
  }

  /**
   * Cleanup all course-times and create the new study-variants taxonomy
   */
  public function courseTimesCleanup() {
    try {
      // check if step has to be performed
      if (!$this->startStep('CourseTimesCleanup')) return true;

      // add study variants
      $this->updateEntityType('taxonomy_term', 'taxonomy_terms/study_variants.taxo');
      $this->updateTerm('study_variants', 'Ganzer Tag: MO');
      $this->updateTerm('study_variants', 'Ganzer Tag: DI');
      $this->updateTerm('study_variants', 'Ganzer Tag: MI');
      $this->updateTerm('study_variants', 'Ganzer Tag: DO');
      $this->updateTerm('study_variants', 'Ganzer Tag: FR');
      $this->updateTerm('study_variants', 'Ganzer Tag: SA');
      $this->updateTerm('study_variants', 'Ganzer Tag: MO, MI, FR');
      $this->updateTerm('study_variants', 'Ganzer Tag: MO, FR, SA');
      $this->updateTerm('study_variants', 'Abend: MO, MI');
      $this->updateTerm('study_variants', 'Abend: MO, DO');
      $this->updateTerm('study_variants', 'NA & Abend: MI');
      $this->updateTerm('study_variants', 'Informationsanlass');

      // update course times node
      $this->updateEntityType('node', 'nodes/course_times.node', array(
        'field_no_vacancy',
        'field_taken_places',
        'field_minimum',
        'field_maximum',
        'field_brand',
        'field_internal_id',
        'field_additional_info_price',
      ));
      $this->updateLocationReferencesOnBundle('course_times', false);
      $this->updateView('views/course_times.view');
      $this->updateView('views/course_times_search.view');

      return $this->endStep('CourseTimesCleanup');
    }
    catch (Exception $ex) {
      drupal_set_message(t('CourseTimesCleanup failed: @msg', array('@msg' => $ex->getMessage())), 'error');
      return false;
    }
  }

  /**
   * Cleanup all locations and brands.
   */
  public function locationAndBrandsCleanup() {
    try {
      // check if step has to be performed
      if (!$this->startStep('LocationAndBrandsCleanup')) return true;

      // delete unused locations
      $this->cleanupLocations();

      // delete unused brands
      $this->deleteTerm('brands', 'KMZ');
      $this->deleteTerm('brands', 'HMZ');
      $this->updateTerm('brands', 'HBS', 'HBSKMZ');

      return $this->endStep('LocationAndBrandsCleanup');
    }
    catch (Exception $ex) {
      drupal_set_message(t('LocationAndBrandsCleanup failed: @msg', array('@msg' => $ex->getMessage())), 'error');
      return false;
    }
  }

  /**
   * Extends banners with new fields and functionality.
   */
  public function bannerExtension() {
    try {
      // check if step has to be performed
      if (!$this->startStep('BannerExtension')) return true;

      // add steps here...
      $this->updateEntityType('node', 'nodes/banner.node');
      $this->updateView('views/banners.view');

      return $this->endStep('BannerExtension');
    }
    catch (Exception $ex) {
      drupal_set_message(t('BannerExtension failed: @msg', array('@msg' => $ex->getMessage())), 'error');
      return false;
    }
  }

  /**
   * Update all changed views and delete the views in db (views only in code)
   */
  public function viewsUpdate() {
    try {
      // check if step has to be performed
      if (!$this->startStep('ViewsUpdate')) return true;

      // add additionalviews here...
      $this->updateView('views/content_by_internal_id.view');

      // clear views cache
      if (module_exists('views')) {
        views_invalidate_cache();
      }

      return $this->endStep('ViewsUpdate');
    }
    catch (Exception $ex) {
      drupal_set_message(t('ViewsUpdate failed: @msg', array('@msg' => $ex->getMessage())), 'error');
      return false;
    }
  }


  //
  // Contact helper functions
  //
  private function combineContacts() {
    //
    // precondition: contact content type adapted
    //
    // combines all contact objects of the same person into a single contact object
    // with all references to locations and courses (multi-fields).
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'contact')
      ->propertyOrderBy('title');
    $result = $query->execute();

    if (isset($result['node'])) {
      $contacts = node_load_multiple(array_keys($result['node']));

      //
      // create contacts grouped by title (name) and combine locations and courses
      // into one contact
      $grouped_contacts = array();
      $location_nids = array();
      $course_nids = array();
      $delete_contacts = array();
      $picture = false;
      foreach ($contacts as $nid => $contact) {
        //
        // delete unpublished contacts and store picture, if any
        if (!$contact->status) {
          $delete_contacts[] = $contact->nid;
          $picture = (isset($contact->field_picture[LANGUAGE_NONE][0]['fid'])) ? $contact->field_picture[LANGUAGE_NONE][0] : false;
          continue;
        }
        else if (!$picture) {
          $picture = (isset($contact->field_picture[LANGUAGE_NONE][0]['fid'])) ? $contact->field_picture[LANGUAGE_NONE][0] : false;
        }

        //
        // start combining contacts of the same person
        if (!array_key_exists($contact->title, $grouped_contacts)) {
          $grouped_contacts[$contact->title] = $contact;
          $location_nids = array();
          $course_nids = array();
        }
        else {
          $delete_contacts[] = $contact->nid;
        }
        $grouped_contact = &$grouped_contacts[$contact->title];

        // set picture, if any
        if (!isset($grouped_contact->field_picture[LANGUAGE_NONE][0]['fid']) && $picture) {
          $grouped_contact->field_picture[LANGUAGE_NONE][0] = $picture;
        }

        // combine locations
        if (!isset($grouped_contact->field_locations[LANGUAGE_NONE])) {
          $grouped_contact->field_locations = array(LANGUAGE_NONE => array());
        }
        $loc_nid = $contact->field_location[LANGUAGE_NONE][0]['target_id'];
        if (!in_array($loc_nid, $location_nids)) {
          $location_nids[] = $loc_nid;
          $grouped_contact->field_locations[LANGUAGE_NONE][] = $contact->field_location[LANGUAGE_NONE][0];
        }

        // combine courses
        if (!isset($grouped_contact->field_courses[LANGUAGE_NONE])) {
          $grouped_contact->field_courses = array(LANGUAGE_NONE => array());
        }
        $course_nid = $contact->field_course[LANGUAGE_NONE][0]['target_id'];
        if (!in_array($course_nid, $course_nids)) {
          $course_nids[] = $course_nid;
          $grouped_contact->field_courses[LANGUAGE_NONE][] = $contact->field_course[LANGUAGE_NONE][0];
        }

      }

      //
      // delete all merged contacts in the db
      node_delete_multiple($delete_contacts);

      //
      // store grouped contacts to db
      foreach ($grouped_contacts as $title => $contact) {
        node_save($contact);
      }
    }
  }

  //
  // Location helper functions
  //
  /**
   * Updates the bundle containing a location reference field with the new location references.
   */
  private function updateLocationReferencesOnBundle($bundle, $multi_field = FALSE) {
    // update old location references for nodes of type $bundle
    foreach ($this->location_mapping as $nid_old => $nid_new) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', $bundle)
        ->propertyCondition('status', 1);
      if ($multi_field) {
        $query->fieldCondition('field_locations', 'target_id', $nid_old);
      }
      else {
        $query->fieldCondition('field_location', 'target_id', $nid_old);
      }
      $result = $query->execute();

      if (isset($result['node'])) {
        $nodes = node_load_multiple(array_keys($result['node']));
        foreach ($nodes as $nid => $node) {
          $node->field_location[LANGUAGE_NONE][0]['target_id'] = $nid_new;
          node_save($node);
        }
      }
    }
  }

  /**
   * Deletes not anymore used location nodes.
   */
  private function cleanupLocations() {
    // delete unused locations
    $loc_nids = array_keys($this->location_mapping);
    node_delete_multiple($loc_nids);

    // rename HBS Rapperswil -> HBSKMZ Rapperswil
    $node = node_load(6687);
    $node->title = 'HBSKMZ Rapperswil';
    node_save($node);
  }


} // end HSORefactoring class
