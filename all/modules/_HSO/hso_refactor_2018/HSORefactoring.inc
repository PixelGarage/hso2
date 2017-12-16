<?php
/**
 * Created by PhpStorm.
 * User: ralph
 * Date: 27.07.17
 * Time: 17:38
 */


class HSORefactoring extends Refactoring {

  private $course_segment_mapping = array(
    // published
    587 => array('segment' => 'Grundbildung KV & Retail', 'category' => 'Vorkurse KV & Retail'), // Kaufmännischer Vorkurs
    598 => array('segment' => 'Grundbildung KV & Retail', 'category' => 'Vorkurse KV & Retail'), // Kaufm. Basisjahr KV (Bürofachdiplom VSH)
    608 => array('segment' => 'Grundbildung KV & Retail', 'category' => 'Eidg. Fähigkeitszeugnisse'), // Kauffrau / Kaufmann (Handelsdiplom VSH & Praktikum)
    611 => array('segment' => 'Grundbildung KV & Retail', 'category' => 'Eidg. Fähigkeitszeugnisse'), // Kauffrau / Kaufmann mit EFZ (B und E Profil)
    613 => array('segment' => 'Grundbildung KV & Retail', 'category' => 'Eidg. Fähigkeitszeugnisse'), // Detailhandelsfachfrau/mann mit EFZ
    6707 => array('segment' => 'Grundbildung KV & Retail', 'category' => 'Eidg. Fähigkeitszeugnisse'), // Kauffrau / Kaufmann mit Berufsmaturität (M-Profil)
    75510 => array('segment' => 'Handelsschule', 'category' => 'Verbandsspezifische Abschlüsse'), // Bürofach- und Handelsdiplom VSH
    75544 => array('segment' => 'Handelsschule', 'category' => 'Verbandsspezifische Abschlüsse'), // Bürofachdiplom VSH
    75545 => array('segment' => 'Handelsschule', 'category' => 'Verbandsspezifische Abschlüsse'), //  Handelsdiplom VSH
    654 => array('segment' => 'Wirtschaft & Management', 'category' => 'Vorkurse & Zertifizierungen'), // Höheres Wirtschaftsdiplom VSK/HWD
    77925 => array('segment' => 'Wirtschaft & Management', 'category' => 'Vorkurse & Zertifizierungen'), // Digital Marketing Assistent/in VSK
    622 => array('segment' => 'Wirtschaft & Management', 'category' => 'Vorkurse & Zertifizierungen'), // Kadervorkurs Management
    736 => array('segment' => 'Wirtschaft & Management', 'category' => 'Vorkurse & Zertifizierungen'), // Kadervorkurs Finance
    8369 => array('segment' => 'Wirtschaft & Management', 'category' => 'Vorkurse & Zertifizierungen'), // Kadervorkurs Englisch
    724 => array('segment' => 'Wirtschaft & Management', 'category' => 'Vorkurse & Zertifizierungen'), // Leadership Zertifikat SVF
    1250 => array('segment' => 'Wirtschaft & Management', 'category' => 'Vorkurse & Zertifizierungen'), // Zertifikatsprüfung Personalassistent/in HRSE
    1249 => array('segment' => 'Wirtschaft & Management', 'category' => 'Vorkurse & Zertifizierungen'), // Sachbearbeiter/in Rechnungswesen
    643 => array('segment' => 'Wirtschaft & Management', 'category' => 'Eidg. Fachausweise & Diplome'), // Direktionsassistent/in mit eidg. Fachausweis
    700 => array('segment' => 'Wirtschaft & Management', 'category' => 'Eidg. Fachausweise & Diplome'), // Führungsfachfrau/fachmann mit eidg. Fachausweis
    1358 => array('segment' => 'Wirtschaft & Management', 'category' => 'Eidg. Fachausweise & Diplome'), // Spezialist/in Unternehmensorganisation mit eidg. Fachausweis
    1359 => array('segment' => 'Wirtschaft & Management', 'category' => 'Eidg. Fachausweise & Diplome'), // Finanzfachfrau/fachmann mit eidg. Fachausweis
    77556 => array('segment' => 'Wirtschaft & Management', 'category' => 'Eidg. Fachausweise & Diplome'), // Technische Kaufleute mit eidg. Fachausweis mit kaufmännischer Vorbildung
    77646 => array('segment' => 'Wirtschaft & Management', 'category' => 'Eidg. Fachausweise & Diplome'), // Technische Kaufleute mit eidg. Fachausweis ohne kaufmännische Vorbildung
    845 => array('segment' => 'Wirtschaft & Management', 'category' => 'Höhere Fachschule HF & Nachdiplomstudium HF'), // Dipl. Betriebswirtschafter/in HF
    8530 => array('segment' => 'Wirtschaft & Management', 'category' => 'Höhere Fachschule HF & Nachdiplomstudium HF'), // Dipl. Betriebswirtschafter/in HF (verkürzt)
    75988 => array('segment' => 'Wirtschaft & Management', 'category' => 'Höhere Fachschule HF & Nachdiplomstudium HF'), // Dipl. Marketingmanager/in HF
    75989 => array('segment' => 'Wirtschaft & Management', 'category' => 'Höhere Fachschule HF & Nachdiplomstudium HF'), // Dipl. Marketingmanager/in HF (verkürzt)
    871 => array('segment' => 'Wirtschaft & Management', 'category' => 'Höhere Fachschule HF & Nachdiplomstudium HF'), // Dipl. Betriebswirtschafter/in NDS HF
    908 => array('segment' => 'Wirtschaft & Management', 'category' => 'Höhere Fachschule HF & Nachdiplomstudium HF'), // Dipl. Marketing- und Verkaufsleiter/in NDS HF
    76041 => array('segment' => 'Wirtschaft & Management', 'category' => 'Höhere Fachschule HF & Nachdiplomstudium HF'), // Dipl. Projektmanager/in NDS HF
    912 => array('segment' => 'Wirtschaft & Management', 'category' => 'Fachhochschule HFH'), // Bachelor of Arts in Betriebswirtschaft (B.A.)
    929 => array('segment' => 'Wirtschaft & Management', 'category' => 'Executive'), // EMBA (ACBSP akkreditiert)
    39371 => array('segment' => 'Wirtschaftsinformatik & Informatik', 'category' => 'Eidg. Fachausweise & Diplome'), // Wirtschaftsinformatiker/in mit eidg. Fachausweis
    36846 => array('segment' => 'Wirtschaftsinformatik & Informatik', 'category' => 'Höhere Fachschule HF & Nachdiplomstudium HF'), // Dipl. Wirtschaftsinformatiker/in HF
    43401 => array('segment' => 'Wirtschaftsinformatik & Informatik', 'category' => 'Höhere Fachschule HF & Nachdiplomstudium HF'), // Dipl. Wirtschaftsinformatiker/in HF (verkürzt)
    //77677 => array('segment' => , 'category' => 63), // Anleitung_EMBA_Alumni

    // unpublished
    /*
    6760 => array('segment' => 1, 'category' => 63), // Kaufmännisches Berufsvorbereitungsjahr
    479 => array('segment' => 2, 'category' => 63), // ECDL Prüfung
    508 => array('segment' => 2, 'category' => 63), // ECDL Core Modul 2 (Windows 7)
    521 => array('segment' => 2, 'category' => 63), // ECDL Core Modul 3 (MS Word)
    534 => array('segment' => 2, 'category' => 63), // ECDL Core Modul 4 (MS Excel)
    547 => array('segment' => 2, 'category' => 63), // ECDL Core Modul 6 (MS Powerpoint)
    558 => array('segment' => 2, 'category' => 63), // ECDL Core Modul 7 (MS Outlook & Internet)
    562 => array('segment' => 2, 'category' => 63), // ECDL Start mit Powerpoint
    572 => array('segment' => 2, 'category' => 63), // ECDL Start mit Outlook
    4948 => array('segment' => 2, 'category' => 63), // ECDL Base Textverarbeitung (MS Word)
    4949 => array('segment' => 2, 'category' => 63), // ECDL Base Tabellenkalkulation (MS Excel)
    4950 => array('segment' => 2, 'category' => 63), // ECDL Base Computer Grundlagen (Windows)
    4951 => array('segment' => 2, 'category' => 63), // ECDL Base Online Grundlagen
    4952 => array('segment' => 2, 'category' => 63), // ECDL Standard Datenbanken anwenden (MS Access)
    4953 => array('segment' => 2, 'category' => 63), // ECDL Standard Präsentation (MS PowerPoint)
    4954 => array('segment' => 2, 'category' => 63), // ECDL Standard Online Zusammenarbeit
    4955 => array('segment' => 2, 'category' => 63), // ECDL Standard IT-Sicherheit
    4956 => array('segment' => 2, 'category' => 63), // ECDL Standard Bildbearbeitung (Gimp)
    4957 => array('segment' => 2, 'category' => 63), // ECDL Base
    4958 => array('segment' => 2, 'category' => 63), // ECDL Standard
    747 => array('segment' => 2, 'category' => 63), // Bürofachdiplom VSH
    6776 => array('segment' => 2, 'category' => 63), // Bürofachdiplom VSH
    8634 => array('segment' => 2, 'category' => 63), // Bürofachdiplom VSH
    75580 => array('segment' => 2, 'category' => 63), // Bürofachdiplom VSH
    4348 => array('segment' => 2, 'category' => 63), // Handelsdiplom VSH (Kompakt)
    6301 => array('segment' => 2, 'category' => 63), // Handelsdiplom VSH (Klassisch)
    6894 => array('segment' => 2, 'category' => 63), // Handelsdiplom VSH
    75581 => array('segment' => 2, 'category' => 63), // Handelsdiplom VSH
    8238 => array('segment' => 2, 'category' => 63), // Vorkurs Grundlagen Computer
    8239 => array('segment' => 2, 'category' => 63), // Vorkurs Allgemeines Rechnen
    8240 => array('segment' => 2, 'category' => 63), // Vorkurs Deutsch
    627 => array('segment' => 3, 'category' => 64), // Technische Kaufleute mit eidg. Fachausweis 2 Semester (Kombi)
    634 => array('segment' => 3, 'category' => 64), // Technische Kaufleute mit eidg. Fachausweis (Direkt)
    734 => array('segment' => 3, 'category' => 64), // Kadervorkurs Leadership
    1242 => array('segment' => 3, 'category' => 65), // Eidg. dipl. Marketingleiter/in
    1243 => array('segment' => 3, 'category' => 65), // Eidg. dipl. Verkaufsleiter/in
    1247 => array('segment' => 3, 'category' => 65), // Marketingfachfrau/-fachmann mit eidg. Fachausweis
    //1248 => array('segment' => , 'category' => ), // Verkaufsfachfrau/-fachmann mit eidg. Fachausweis
    1255 => array('segment' => 3, 'category' => 65), // Zertifikatsprüfung Markom
    1360 => array('segment' => 3, 'category' => 67), // HR-Fachfrau/-fachmann mit eidg. Fachausweis
    4760 => array('segment' => 3, 'category' => 64), // Prüfungstrainings Eidg. Berufsprüfungen
    7742 => array('segment' => 3, 'category' => 64), // Legal Office Manager (HSO/Lawbility zertifiziert)
    7818 => array('segment' => 3, 'category' => 65), // Tourismus Agent (Zertifikat HSO/SHT)
    7838 => array('segment' => 3, 'category' => 65), // Tourismus Manager (Diplom HSO/SHT)
    8241 => array('segment' => 3, 'category' => 63), // Finanzfachfrau/-fachmann mit eidg. Fachausweis
    12737 => array('segment' => 3, 'category' => 64), // Technische Kaufleute mit eidg. Fachausweis 3 Semester (Kombi)
    33306 => array('segment' => 3, 'category' => 63), // Test Course Altestetten
    75513 => array('segment' => 3, 'category' => 63), // Verkaufsfachfrau/-fachmann mit eidg. Fachausweis
    75783 => array('segment' => 108, 'category' => 87), // Dipl. Projektmanager/in NDS HF
    7994 => array('segment' => 108, 'category' => 87), // Dipl. Logistikleiter/-in NDS HF
    8013 => array('segment' => 108, 'category' => 87), // Dipl. Expertin / Experte in Organisationsentwicklung NDS HF
    */
  );

  /**
   * Executes all refactoring.
   */
  public function runAll() {
    // call parent function
    parent::runAll();

    //__hso_refactor_2018_node_export('benefit');
    //return true;

    // enable new modules
    if (!module_exists('views_bulk_operations')) {
      //module_enable(array('views_bulk_operations'), true);
    }

    //
    // run all refactoring step by step. Each step has to succeed
    // to continue refactoring
    //if (!$this->addNewSegmentsAndCategories()) return 'AddNewSegmentsAndCategories';
    //if (!$this->updateCourseLayout()) return 'UpdateCourseLayout';
    //if (!$this->importCall2Actions()) return 'ImportCall2Actions';
    //if (!$this->importBenefits()) return 'ImportBenefits';
    if (!$this->importHBSKMZCourses()) return 'ImportHBSKMZCourses';
    //if (!$this->bannerExtension()) return 'BannerExtension';
    //if (!$this->cleanupContext()) return 'CleanupContext';
    //if (!$this->viewsUpdate()) return 'ViewsUpdate';

    //
    // display hints for manual update
    drupal_set_message('Manual update: import main menu from file main-menu-export!', 'warning');
    drupal_set_message('Manual update: import menu course-blocks from file menu-course-blocks-export.txt!', 'warning');
    drupal_set_message('Manual update: Add DS block fields Course benefits, Course info, Course links!', 'warning');
    drupal_set_message('Manual update: remove exposed search block from navigation region!', 'warning');
    drupal_set_message('Manual update: Home-Page refresh!', 'warning');

    // refactoring successfully completed
    return TRUE;
  }

  /**
   * Resets a given step.
   */
  public function resetStep($step_name) {
    // reset contact cleanup, if not finished
    if ($step_name == 'BannerExtension') {
      // make sure that newly added fields are completely deleted (also DB tables)
      $this->deleteFields(array('field_text_orientation'));
    }

  }


  //
  // Steps implementation
  //
  /**
   * Cleanup all course-times and create the new study-variants taxonomy
   */
  public function addNewSegmentsAndCategories() {
    try {
      // check if step has to be performed
      if (!$this->startStep('AddNewSegmentsAndCategories')) return true;

      // add new segments
      $this->updateTerm('segments', 'Wirtschaft & Management');
      $this->updateTerm('segments', 'Wirtschaftsinformatik & Informatik');
      $this->updateTerm('segments', 'Handelsschule');
      $this->updateTerm('segments', 'Grundbildung KV & Retail');
      $this->updateTerm('segments', 'Sprachen');
      $this->updateTerm('segments', 'Professional Programs');

      // add new course categories
      // - Wirtschaft & Management
      // - Wirtschaftsinformatik & Informatik
      $this->updateTerm('menugroup', 'Vorkurse & Zertifizierungen');
      $this->updateTerm('menugroup', 'Eidg. Fachausweise & Diplome');
      $this->updateTerm('menugroup', 'Höhere Fachschule HF & Nachdiplomstudium HF');
      $this->updateTerm('menugroup', 'Fachhochschule HFH');
      $this->updateTerm('menugroup', 'Executive');
      // - Handelsschule
      $this->updateTerm('menugroup', 'Verbandsspezifische Abschlüsse');

      // - Grundbildung KV & Retail
      $this->updateTerm('menugroup', 'Vorkurse KV & Retail');
      $this->updateTerm('menugroup', 'Eidg. Fähigkeitszeugnisse');

      // - Professional Programs
      $this->updateTerm('menugroup', 'Wirtschaft & Management-Zertifikate');
      $this->updateTerm('menugroup', 'Wirtschaftsinformatik & Informatik-Zertifikate');
      $this->updateTerm('menugroup', 'Firmenkunden');

      // map courses to new segments
      $this->_mapCoursesToNewSegmentsAndCategories();

      // delete old segments
      $this->deleteTerm('segments', 'KV College');
      $this->deleteTerm('segments', 'Kaderschule');
      $this->deleteTerm('segments', 'Höhere Fachschule');
      $this->deleteTerm('segments', 'Fachhochschule');
      $this->deleteTerm('segments', 'Executive');

      // delete old course categories
      $this->deleteTerm('menugroup', 'Diplomstudium HF');
      $this->deleteTerm('menugroup', 'Diplomstudium HF (verkürzt)');
      $this->deleteTerm('menugroup', 'Bachelor-Studium');
      $this->deleteTerm('menugroup', 'Nachdiplomstudiengänge HF ');
      $this->deleteTerm('menugroup', 'Management und Organisation');
      $this->deleteTerm('menugroup', 'Leadership und Personal');
      $this->deleteTerm('menugroup', 'Finanzen');
      $this->deleteTerm('menugroup', 'Marketing und Verkauf');
      $this->deleteTerm('menugroup', 'Wirtschaftsinformatik');
      $this->deleteTerm('menugroup', 'Höhere Fachschule für Wirtschaft');
      $this->deleteTerm('menugroup', 'Höhere Fachschule für Wirtschaftsinformatik');

      return $this->endStep('AddNewSegmentsAndCategories');
    }
    catch (Exception $ex) {
      drupal_set_message(t('AddNewSegmentsAndCategories() failed: @msg', array('@msg' => $ex->getMessage())), 'error');
      return false;
    }
  }

  /**
   * Update course layout and add sidebar blocks.
   */
  public function updateCourseLayout() {
    try {
      // check if step has to be performed
      if (!$this->startStep('UpdateCourseLayout')) return true;

      // update course content type
      $this->updateEntityType('node', 'nodes/course.node');

      return $this->endStep('UpdateCourseLayout');
    }
    catch (Exception $ex) {
      drupal_set_message(t('UpdateCourseLayout() failed: @msg', array('@msg' => $ex->getMessage())), 'error');
      return false;
    }
  }

  /**
   * Import all new call2action nodes from json-file.
   */
  public function importCall2Actions() {
    try {
      // check if step has to be performed
      if (!$this->startStep('ImportCall2Actions')) return true;

      // update call2action content type
      $this->updateEntityType('node', 'nodes/call2action.node');

      // import nodes in json file
      __hso_refactor_2018_node_import('call2action', true);

      // update view
      $this->updateView('views/call2action.view');

      return $this->endStep('ImportCall2Actions');
    }
    catch (Exception $ex) {
      drupal_set_message(t('ImportCall2Actions() failed: @msg', array('@msg' => $ex->getMessage())), 'error');
      return false;
    }
  }

  /**
   * Import all new benefit nodes from json-file.
   */
  public function importBenefits() {
    try {
      // check if step has to be performed
      if (!$this->startStep('ImportBenefits')) return true;

      // update call2action content type
      $this->updateEntityType('node', 'nodes/benefit.node');

      // import nodes in json file
      __hso_refactor_2018_node_import('benefit', true);

      // update view
      $this->updateView('views/benefits.view');

      return $this->endStep('ImportBenefits');
    }
    catch (Exception $ex) {
      drupal_set_message(t('ImportBenefits() failed: @msg', array('@msg' => $ex->getMessage())), 'error');
      return false;
    }
  }

  /**
   * Import all language courses, including the course-times and tabs of each course from HBBS-KMZ.
   */
  public function importHBSKMZCourses() {
    try {
      // check if step has to be performed
      if (!$this->startStep('ImportHBSKMZCourses')) return true;

      // import price page from json file
     $nids = __hso_refactor_2018_node_import('page');

      // import all exported files
      _hso_refactor_2018_hbskmz_import($nids[0]);

      return $this->endStep('ImportHBSKMZCourses');
    }
    catch (Exception $ex) {
      drupal_set_message(t('ImportHBSKMZCourses() failed: @msg', array('@msg' => $ex->getMessage())), 'error');
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
      $this->_updateBanners();
      $this->updateView('views/banners.view');

      return $this->endStep('BannerExtension');
    }
    catch (Exception $ex) {
      drupal_set_message(t('BannerExtension failed: @msg', array('@msg' => $ex->getMessage())), 'error');
      return false;
    }
  }

  /**
   * Cleanup of all contexts.
   */
  public function cleanupContext() {
    try {
      // check if step has to be performed
      if (!$this->startStep('CleanupContext')) return true;

      // delete old contexts
      $this->deleteContext('ex-college');
      $this->deleteContext('ex-college-home');
      $this->deleteContext('fh-college');
      $this->deleteContext('fh-college-home');
      $this->deleteContext('hf-college');
      $this->deleteContext('hf-college-home');
      $this->deleteContext('hs-college');
      $this->deleteContext('hs-college-home');
      $this->deleteContext('ks-college');
      $this->deleteContext('ks-college-home');
      $this->deleteContext('kv-college');
      $this->deleteContext('kv-college-home');

      // import new contexts
      $this->importContext('contexts/segment_wm.cont');
      $this->importContext('contexts/segment_wm_home.cont');
      $this->importContext('contexts/segment_wi.cont');
      $this->importContext('contexts/segment_wi_home.cont');
      $this->importContext('contexts/segment_hs.cont');
      $this->importContext('contexts/segment_hs_home.cont');
      $this->importContext('contexts/segment_kv.cont');
      $this->importContext('contexts/segment_kv_home.cont');
      $this->importContext('contexts/segment_sp.cont');
      $this->importContext('contexts/segment_sp_home.cont');
      $this->importContext('contexts/segment_pp.cont');
      $this->importContext('contexts/segment_pp_home.cont');
      $this->importContext('contexts/segments_home.cont');
      $this->importContext('contexts/course.cont');

      return $this->endStep('CleanupContext');
    }
    catch (Exception $ex) {
      drupal_set_message(t('CleanupContext() failed: @msg', array('@msg' => $ex->getMessage())), 'error');
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

      // add additional views here...
      $this->updateView('views/courses.view');
      $this->updateView('views/course_times.view');
      $this->updateView('views/course_times_search.view');
      $this->updateView('views/news.view');
      $this->updateView('views/locations.view');
      $this->updateView('views/segment_pages.view');

      // clear views cache
      if (module_exists('views')) {
        views_invalidate_cache();
      }

      return $this->endStep('ViewsUpdate');
    }
    catch (Exception $ex) {
      drupal_set_message(t('ViewsUpdate() failed: @msg', array('@msg' => $ex->getMessage())), 'error');
      return false;
    }
  }



  //
  // Helper functions
  //
  private function _mapCoursesToNewSegmentsAndCategories() {
    foreach ($this->course_segment_mapping as $course_nid => $term_names) {
      $segment_terms = taxonomy_get_term_by_name($term_names['segment'], 'segments');
      $segment_tid = array_keys($segment_terms)[0];
      $category_terms = taxonomy_get_term_by_name($term_names['category'], 'menugroup');
      $category_tid = array_keys($category_terms)[0];
      $num = db_update('field_data_field_segment')
        ->fields(array('field_segment_tid' => $segment_tid))
        ->condition('entity_id', $course_nid)
        ->execute();
      $num = db_update('field_data_field_fachgebiet')
        ->fields(array('field_fachgebiet_tid' => $category_tid))
        ->condition('entity_id', $course_nid)
        ->execute();
    }
  }

  private function _updateBanners() {
    $banners = null;
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'banner')
      ->propertyCondition('status', NODE_PUBLISHED);
    $result = $query->execute();
    if (isset($result['node'])) {
      $banners = node_load_multiple(array_keys($result['node']));
    }

    foreach ($banners as $key => $banner) {
      $banner->field_text_orientation[LANGUAGE_NONE][0]['value'] = 'left';
      $old_segment_tid = $banner->field_segment[LANGUAGE_NONE][0]['tid'];
      switch ($old_segment_tid) {
        case 1:
          $term_name = 'Grundbildung KV & Retail';
          break;
        case 2:
          $term_name = 'Handelsschule';
          break;
        case 3:
          $term_name = 'Wirtschaft & Management';
          break;
        case 4:
          $term_name = 'Professional Programs';
          break;
        case 108:
          $term_name = 'Wirtschaftsinformatik & Informatik';
          break;
        case 109:
        $term_name = 'Sprachen';
          break;
        default:
          $term_name = '';
      }
      if ($term_name) {
        $segment_terms = taxonomy_get_term_by_name($term_name, 'segments');
        $banner->field_segment[LANGUAGE_NONE][0]['tid'] = array_keys($segment_terms)[0];
      }
      node_save($banner);
    }
  }

} // end HSORefactoring class
