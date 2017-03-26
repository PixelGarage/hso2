<?php

class HSO {
  private $db;

  private $connected = FALSE;

  /**
   * Initialisation via web service.
   */
  public function init() {
    $live_import = array_key_exists('livedb', $_GET);
    $this->initDB($live_import);
  }

  public function initDB($live_import = TRUE) {
    $db_name = $live_import ? 'hsoch_anmSysNew' : 'hsoch_anmSysTest';
    $this->db = new Mysqli('localhost', 'hsoch_asUser', 'a100ce5s2db', $db_name);

    // validate DB connection
    if ($this->db->connect_errno) {
      // no DB connection possible
      error_log("HSO Importer error: DB import system could not be connected", 0);
      $this->connected = FALSE;

    }
    else {
      // successful DB connection
      error_log("HSO Importer status: DB '" . $db_name . "' connected", 0);
      $this->db->query('SET NAMES utf8');
      $this->connected = TRUE;

    }

    return $this->connected;
  }

  /**
   * Imports the CSV files from the HERMES system.
   * The function checks the files before the import is started. In case of
   * an error, the function returns FALSE, and logs the error.
   *
   * @return bool
   *    True if the import of the CSV files was successful, FALSE otherwise.
   */
  public function anmsysImport() {
    //
    // check if DB is connected
    if (!$this->connected) {
      watchdog("ANMSYS Importer", "ANMSYS Importer error: DB is not connected", NULL, WATCHDOG_ERROR);
      return FALSE;
    }

    //
    // Define csv transfer folder
    $csv_path = "../anmsys_transfer"; // outside Drupal root directory

    //
    // Header definition of csv files in Anmeldesystem
    $files['V_ANMSYS_EXP_Durchfuehrungen']['expected_count'] = 5;
    $files['V_ANMSYS_EXP_Durchfuehrungen']['header_keys'] = array('df_id', 'location', 'start_date', 'start_time', 'end_date', 'nof_tn', 'durchf_text', 'lehrgang_id', 'price_brutto', 'price_fsv_reduce', 'price_netto', 'branch_id', 'webTemplate', 'teilnehmer_min', 'teilnehmer_max', 'webPriceRowFormat', 'webPriceAdditional');
    $files['V_ANMSYS_EXP_Ansprechperson']['expected_count'] = 5;
    $files['V_ANMSYS_EXP_Ansprechperson']['header_keys'] = array('branch_id', 'department_id', 'lehrgang_id', 'fk_modul_id', 'show_value', 'contact_person', 'contact_tel', 'contact_email');
    $files['V_ANMSYS_EXP_Department']['expected_count'] = 3;
    $files['V_ANMSYS_EXP_Department']['header_keys'] = array('department_id', 'department_short', 'department_long');
    $files['V_ANMSYS_EXP_Lehrgaenge']['expected_count'] = 5;
    $files['V_ANMSYS_EXP_Lehrgaenge']['header_keys'] = array('lehrgang_id', 'bezeichnung_kurz', 'department_id', 'webRegistrationTitle', 'webRegistrationTitleMeta', 'webRegistrationShortDescription', 'webRegistrationPageMeta', 'type');
    $files['V_ANMSYS_EXP_Branches']['expected_count'] = 5;
    $files['V_ANMSYS_EXP_Branches']['header_keys'] = array('branch_id', 'brach_short', 'branch_long', 'branch_town', 'branch_state', 'branch_address', 'branch_zip', 'branch_phone', 'branch_fax', 'branch_mail', 'standort_id', 'standort_name', 'brand_id', 'brand_short');

    //
    // quick check, if number of rows in files exceed minimal row number
    $error = false;
    $error_log = 'ANMSYS Importer error: ';
    foreach ($files as $key => $element) {
      $count = exec("cat $csv_path/$key.csv | wc -l");
      if ($count < $element['expected_count']) {
        $error_log .= "Abbruch: Anzahl {$key} ({$count}) war kleiner als erwartet ({$element['expected_count']})\n";
        $error = true;
      }
    }

    if ($error) {
      watchdog("ANMSYS Importer", $error_log, NULL, WATCHDOG_ERROR);
      return FALSE;
    }

    //
    // truncate transfer tables
    //mysql_query("TRUNCATE anm_h4_durchfuehrungen");
    $this->db->query("TRUNCATE V_ANMSYS_EXP_Durchfuehrungen");
    $this->db->query("TRUNCATE V_ANMSYS_EXP_Ansprechperson");
    $this->db->query("TRUNCATE V_ANMSYS_EXP_Department");
    $this->db->query("TRUNCATE V_ANMSYS_EXP_Lehrgaenge");
    $this->db->query("TRUNCATE V_ANMSYS_EXP_Branches");

    //
    // import data from CSV files into transfer tables
    foreach ($files as $key => $element) {
      $filepath = "$csv_path/$key.csv";
      $csv_rows = array_map('str_getcsv', file($filepath));
      $sql_query = "INSERT INTO $key (" . implode(',', $element['header_keys']) . ") VALUES (";

      foreach ($csv_rows as $idx => $row) {
        $row_str = implode("', '", $row);
        $conv_row = iconv('windows-1252', 'utf-8', $row_str);
        if(!$conv_row) $conv_row = $row_str;
        $query = $sql_query . "'" . $conv_row . "')";
        $status = $this->db->query($query);
        if (!$status) {
          watchdog("ANMSYS Importer", "Import of CSV file {$key} failed in line @idx", array('@idx' => $idx), WATCHDOG_ERROR);
          return false;
        }
      }
    }

    //
    // Correct some imported data
    // Transformationen
    //$this->db->query("update anm_h4_durchfuehrungen D set lehrgang_id = case when lehrgang_id < pow(2,23) then (select lehrgang_id from hso_anmsys.anm_lehrgang where h4_lg_id = D.lehrgang_id) else (select lehrgang_id from hso_anmsys.anm_lehrgang where h4_m_id = D.lehrgang_id - pow(2,23)) end");
    //$this->db->query("UPDATE sis_import.anm_h4_durchfuehrungen AS DF SET location = (SELECT AL.location_id FROM hso_anmsys_new.anm_location AS AL WHERE DF.location = AL.h4_branch_id)");
    $status = $this->db->query("UPDATE V_ANMSYS_EXP_Durchfuehrungen AS DF SET location = (SELECT AL.location_id FROM anm_location AS AL WHERE DF.location = AL.h4_branch_id)");
    watchdog("ANMSYS Importer", 'V_ANMSYS_EXP_Durchfuehrungen location correction status @status', array('@status' => $status ? 'ok' : 'failed'), WATCHDOG_INFO);
    if (!$status) $error = true;

    //
    // V_ANMSYS_EXP_Branches import to Anmeldesystem
    $result = $this->db->query("SELECT count(*) as cnt FROM V_ANMSYS_EXP_Branches");
    $count = $result->fetch_assoc()['cnt'];
    if ($count > $files['V_ANMSYS_EXP_Branches']['expected_count']) {
      $this->db->query("TRUNCATE TABLE anm_h4_branches");
      $status = $this->db->query("INSERT INTO anm_h4_branches SELECT * FROM V_ANMSYS_EXP_Branches");
      watchdog("ANMSYS Importer", 'anm_h4_branches table import @status', array('@status' => $status ? 'ok' : 'failed'), WATCHDOG_INFO);
      if (!$status) $error = true;
    }
    else {
      watchdog("ANMSYS Importer", 'anm_h4_branches table not imported. No data!', NULL, WATCHDOG_WARNING);
      $error = true;
    }

    //
    // V_ANMSYS_EXP_Durchfuehrungen import to Anmeldesystem
    $result = $this->db->query("SELECT count(*) as cnt FROM V_ANMSYS_EXP_Durchfuehrungen");
    $count = $result->fetch_assoc()['cnt'];
    if ($count > $files['V_ANMSYS_EXP_Durchfuehrungen']['expected_count']) {
      $this->db->query("TRUNCATE TABLE anm_h4_durchfuehrungen");
      $status = $this->db->query("INSERT INTO anm_h4_durchfuehrungen SELECT * FROM V_ANMSYS_EXP_Durchfuehrungen");
      watchdog("ANMSYS Importer", 'anm_h4_durchfuehrungen table import @status', array('@status' => $status ? 'ok' : 'failed'), WATCHDOG_INFO);
      if (!$status) $error = true;
    }
    else {
      watchdog("ANMSYS Importer", 'anm_h4_durchfuehrungen table not imported. No data!', NULL, WATCHDOG_WARNING);
      $error = true;
    }

    //
    // V_ANMSYS_EXP_Ansprechperson import to Anmeldesystem
    $result = $this->db->query("SELECT count(*) as cnt FROM V_ANMSYS_EXP_Ansprechperson");
    $count = $result->fetch_assoc()['cnt'];
    if ($count > $files['V_ANMSYS_EXP_Ansprechperson']['expected_count']) {
      $this->db->query("TRUNCATE TABLE anm_h4_contact");
      $status = $this->db->query("INSERT INTO anm_h4_contact SELECT * FROM V_ANMSYS_EXP_Ansprechperson");
      watchdog("ANMSYS Importer", 'anm_h4_contact table import @status', array('@status' => $status ? 'ok' : 'failed'), WATCHDOG_INFO);
      if (!$status) $error = true;
    }
    else {
      watchdog("ANMSYS Importer", 'anm_h4_contact table not imported. No data!', NULL, WATCHDOG_WARNING);
      $error = true;
    }

    //
    // V_ANMSYS_EXP_Department import to Anmeldesystem
    $result = $this->db->query("SELECT count(*) as cnt FROM V_ANMSYS_EXP_Department");
    $count = $result->fetch_assoc()['cnt'];
    if ($count > $files['V_ANMSYS_EXP_Department']['expected_count']) {
      $this->db->query("TRUNCATE TABLE anm_h4_departments");
      $status = $this->db->query("INSERT INTO anm_h4_departments SELECT * FROM V_ANMSYS_EXP_Department");
      watchdog("ANMSYS Importer", 'anm_h4_departments table import @status', array('@status' => $status ? 'ok' : 'failed'), WATCHDOG_INFO);
      if (!$status) $error = true;
    }
    else {
      watchdog("ANMSYS Importer", 'anm_h4_departments table not imported. No data!', NULL, WATCHDOG_WARNING);
      $error = true;
    }

    //
    // V_ANMSYS_EXP_Lehrgaenge import to Anmeldesystem
   $result = $this->db->query("SELECT count(*) as cnt FROM V_ANMSYS_EXP_Lehrgaenge");
    $count = $result->fetch_assoc()['cnt'];
    if ($count > $files['V_ANMSYS_EXP_Lehrgaenge']['expected_count']) {
      $this->db->query("TRUNCATE TABLE anm_h4_lehrgange");
      $status = $this->db->query("INSERT INTO anm_h4_lehrgange SELECT * FROM V_ANMSYS_EXP_Lehrgaenge");
      watchdog("ANMSYS Importer", 'anm_h4_lehrgange table import @status', array('@status' => $status ? 'ok' : 'failed'), WATCHDOG_INFO);
      if (!$status) $error = true;
    }
    else {
      watchdog("ANMSYS Importer", 'anm_h4_lehrgange table not imported. No data!', NULL, WATCHDOG_WARNING);
      $error = true;
    }

    return !$error;
  }

  /**
   * Returns a JSON string object to the browser when hitting the root of the domain
   *
   * @url GET /
   */
  public function ping() {
    return "Pong";
  }

  /**
   * Gets the segments (departments)
   *
   * @url GET /segments
   */
  public function getSegments() {
    $segments = array();
    if (!$this->connected) {
      return $segments;
    }

    $result = $this->db->query('SELECT department_id, department_long ' .
      'FROM anm_departments ORDER BY department_id ASC');
    while ($row = $result->fetch_object()) {
      $segment = new StdClass();
      $segment->id = $row->department_id;
      $segment->title = $row->department_long;
      $segments[] = $segment;
    }
    return $segments;
  }

  /**
   * Gets the segment (department) by department id
   *
   * @url GET /segments/$id
   */
  public function getSegment($id) {
    $segment = FALSE;
    if (!$this->connected) {
      return $segment;
    }

    $id = intval($id);
    $result = $this->db->query('SELECT department_id, department_long ' .
      'FROM anm_departments WHERE department_id = ' . $id);
    if ($row = $result->fetch_object()) {
      $segment = new StdClass();
      $segment->id = $row->department_id;
      $segment->title = $row->department_long;
    }
    return $segment;
  }

  /**
   * Gets the contacts by segment (department)
   *
   * @url GET /segments/$id/contacts
   */
  public function getContactsBySegment($id) {
    $contacts = array();
    if (!$this->connected) {
      return $contacts;
    }

    $id = intval($id);
    $result = $this->db->query('SELECT c.branch_id, c.department_id, c.show_value, c.contact_person, c.contact_tel, c.contact_email, c.anm_lehrgang_id, ' .
      'b.branch_town, b.branch_address, b.branch_zip, b.branch_fax, b.branch_phone, b.branch_mail, b.brand_id, b.brand_short ' .
      'FROM anm_contacts c JOIN anm_branches b ON (c.branch_id = b.branch_id) ' .
      'WHERE c.department_id = ' . $id . ' ORDER BY b.branch_id, c.anm_lehrgang_id');
    while ($row = $result->fetch_object()) {
      $contact = new StdClass();
      $contact->id = $row->show_value;
      $contact->name = $row->contact_person;
      $contact->phone = $row->contact_tel;
      $contact->email = $row->contact_email;
      // create location object (branch)
      $contact->location = new StdClass();
      $contact->location->id = $row->branch_id;
      $contact->location->name = $row->branch_town;
      $contact->location->address = $row->branch_address;
      $contact->location->plz = $row->branch_zip;
      $contact->location->fax = $row->branch_fax;
      $contact->location->phone = $row->branch_phone;
      $contact->location->email = $row->branch_mail;
      $contact->location->brand_id = $row->brand_id;
      $contact->location->brand_short = $row->brand_short;
      $contacts[] = $contact;
    }
    return $contacts;
  }

  /**
   * Gets the contacts of a course for the brand
   *
   * @url GET /brand/$brand/course/$id/contacts
   */
  public function getContactsOfCourseForBrand($brand, $id) {
    $contacts = array();
    if (!$this->connected) {
      return $contacts;
    }

    $id = intval($id);
    $brand_id = intval($brand);

    // HSO: import all available contacts
    // all other brands: import all contacts of given brand and all contacts of HSO (brand_id = 1) that are located at brand locations
    $and_s_visible_for_brand = ($brand_id == 1) ? '' :
      ' AND (b.brand_id = ' . $brand_id . ' OR b.brand_id = 1 AND b.standort_id IN (SELECT bb.standort_id FROM anm_branches bb WHERE bb.brand_id = ' . $brand_id . '))';

    $result = $this->db->query('SELECT c.branch_id, c.department_id, c.show_value, c.contact_person, c.contact_tel, c.contact_email, c.lehrgang_id, ' .
      'b.branch_town, b.branch_address, b.branch_zip, b.branch_fax, b.branch_phone, b.branch_mail, b.brand_id, b.brand_short ' .
      'FROM anm_contacts c JOIN anm_branches b ON (c.branch_id = b.branch_id) ' .
      'WHERE c.lehrgang_id = ' . $id . $and_s_visible_for_brand . ' ORDER BY b.brand_id, b.branch_id');
    while ($row = $result->fetch_object()) {
      $contact = new StdClass();
      // define unique internal id, no representation on server
      $contact->id = $row->show_value;
      $contact->name = $row->contact_person;
      $contact->phone = $row->contact_tel;
      $contact->email = $row->contact_email;
      $contact->picture = 'public://user_pictures/' . $row->show_value . '.jpg';
      // create location object (branch)
      $contact->location = new StdClass();
      $contact->location->id = $row->branch_id;
      $contact->location->name = $row->branch_town;
      $contact->location->address = $row->branch_address;
      $contact->location->plz = $row->branch_zip;
      $contact->location->fax = $row->branch_fax;
      $contact->location->phone = $row->branch_phone;
      $contact->location->email = $row->branch_mail;
      $contact->location->brand_id = $row->brand_id;
      $contact->location->brand_short = $row->brand_short;
      $contacts[] = $contact;
    }
    return $contacts;
  }

  /**
   * Gets the course id updates. This update has to be performed once.
   *
   * @url GET /courses/idupdates
   */
  public function getCoursesIDUpdates() {
    $course_ids = array();
    if (!$this->connected) {
      return $course_ids;
    }

    $result = $this->db->query('SELECT old_id, new_id FROM anm_change_ids ORDER BY old_id ASC');
    while ($row = $result->fetch_object()) {
      $course_ids[$row->old_id] = $row->new_id;
    }
    return $course_ids;
  }

  /**
   * Gets the courses by segment (department)
   *
   * @url GET /segments/$id/courses
   */
  public function getCoursesBySegment($id) {
    $courses = array();
    if (!$this->connected) {
      return $courses;
    }

    $id = intval($id);
    $result = $this->db->query('SELECT lehrgang_id, department_id, webRegistrationTitle, webRegistrationTitleMeta, ' .
      ' webRegistrationShortDescription, webRegistrationPageMeta, type ' .
      'FROM anm_lehrgaenge ' .
      'WHERE department_id = ' . $id . ' ORDER BY lehrgang_id ASC');
    while ($row = $result->fetch_object()) {
      $course = new StdClass();
      $course->id = $row->lehrgang_id;
      $course->segment_id = $row->department_id;
      $course->title = $row->webRegistrationTitle;
      $course->is_module = $row->type == 'MODUL';
      // create meta object
      $course->meta = new StdClass();
      $course->meta->title = $row->webRegistrationTitleMeta;
      $course->meta->description = $row->webRegistrationShortDescription;
      $course->meta->keywords = $row->webRegistrationPageMeta;
      $courses[] = $course;
    }
    return $courses;
  }

  /**
   * Gets the courses
   *
   * @url GET /courses
   */
  public function getCourses() {
    $courses = array();
    if (!$this->connected) {
      return $courses;
    }

    $result = $this->db->query('SELECT lehrgang_id, department_id, webRegistrationTitle, webRegistrationTitleMeta, ' .
      ' webRegistrationShortDescription, webRegistrationPageMeta, type ' .
      'FROM anm_lehrgaenge ORDER BY lehrgang_id ASC');
    while ($row = $result->fetch_object()) {
      $course = new StdClass();
      $course->id = $row->lehrgang_id;
      $course->segment_id = $row->department_id;
      $course->title = $row->webRegistrationTitle;
      $course->is_module = $row->type == 'MODUL';
      // create meta object
      $course->meta = new StdClass();
      $course->meta->title = $row->webRegistrationTitleMeta;
      $course->meta->description = $row->webRegistrationShortDescription;
      $course->meta->keywords = $row->webRegistrationPageMeta;
      $courses[] = $course;
    }
    return $courses;
  }

  /**
   * Gets the course by id
   *
   * @url GET /courses/$id
   */
  public function getCourse($id) {
    $course = FALSE;
    if (!$this->connected) {
      return $course;
    }

    $id = intval($id);
    $result = $this->db->query('SELECT lehrgang_id, department_id, webRegistrationTitle, webRegistrationTitleMeta, ' .
      ' webRegistrationShortDescription, webRegistrationPageMeta, type ' .
      'FROM anm_lehrgaenge WHERE lehrgang_id = ' . $id);
    if ($row = $result->fetch_object()) {
      $course = new StdClass();
      $course->id = $row->lehrgang_id;
      $course->segment_id = $row->department_id;
      $course->title = $row->webRegistrationTitle;
      $course->is_module = $row->type == 'MODUL';
      // create meta object
      $course->meta = new StdClass();
      $course->meta->title = $row->webRegistrationTitleMeta;
      $course->meta->description = $row->webRegistrationShortDescription;
      $course->meta->keywords = $row->webRegistrationPageMeta;
      $courses[] = $course;
    }
    return $course;
  }

  /**
   * Gets the times of a course for a brand
   *
   * @url GET /brand/$brand/course/$id/times
   */
  public function getTimesOfCourseForBrand($brand, $id) {
    $times = array();
    if (!$this->connected) {
      return $times;
    }

    $id = intval($id);
    $brand_id = intval($brand);

    // HSO: import all available course times
    // all other brands: import all course times of given brand and all course times of HSO (brand_id = 1) that are located at brand locations
    $and_s_visible_for_brand = ($brand_id == 1) ? '' :
      ' AND (b.brand_id = ' . $brand_id . ' OR b.brand_id = 1 AND b.standort_id IN (SELECT bb.standort_id FROM anm_branches bb WHERE bb.brand_id = ' . $brand_id . '))';

    $query = 'SELECT a.df_id, a.start_date, a.start_time, a.end_date, a.durchf_text, a.lehrgang_id, a.nof_tn, a.price_brutto, a.price_netto, ' .
      'a.webTemplate, a.teilnehmer_min, a.teilnehmer_max, a.price_text, a.price_additional, ' .
      'b.branch_id, b.branch_town, b.branch_address, b.branch_zip, b.branch_fax, b.branch_phone, b.branch_mail, b.brand_id, b.brand_short ' .
      'FROM anm_durchfuehrungen_combined a ' .
      'LEFT JOIN anm_branches b ON (a.branch_id = b.branch_id) ' .
      'WHERE a.lehrgang_id = ' . $id . $and_s_visible_for_brand . ' ORDER BY a.start_date ASC';

    $result = $this->db->query($query);
    while ($row = $result->fetch_object()) {
      $time = new StdClass();
      $time->id = $row->df_id;
      $time->course_id = $row->lehrgang_id;
      $time->start_date = substr($row->start_date, 0, 10);
      $time->start_time = empty($row->start_time) ? NULL : str_replace('.', ':', substr(trim($row->start_time), 0, 5)) . ':00';
      $time->end_date = empty($row->end_date) || $row->end_date == '0000-00-00 00:00:00' ? NULL : substr($row->end_date, 0, 10);
      $time->description = $row->durchf_text;
      $time->has_subsidy = $row->price_brutto <> $row->price_netto;
      $time->price_brutto = $row->price_brutto;
      $time->price_netto = $row->price_netto;
      $time->price_detailed = $row->price_text;
      $time->price_additional = $row->price_additional;
      $time->taken_places = $row->nof_tn == -1 ? 0 : $row->nof_tn;
      $time->no_vacancy = $row->nof_tn == -1;
      $time->min_places = $row->teilnehmer_min;
      $time->max_places = $row->teilnehmer_max;
      $time->template = $row->webTemplate;
      $time->brand_id = $row->brand_id;
      // create location object (branch)
      $time->location = new StdClass();
      $time->location->id = $row->branch_id;
      $time->location->name = $row->branch_town;
      $time->location->address = $row->branch_address;
      $time->location->plz = $row->branch_zip;
      $time->location->fax = $row->branch_fax;
      $time->location->phone = $row->branch_phone;
      $time->location->email = $row->branch_mail;
      $time->location->brand_id = $row->brand_id;
      $time->location->brand_short = $row->brand_short;
      $times[] = $time;
    }
    return $times;
  }

  /**
   * Gets the locations (branches)
   *
   * @url GET /locations
   */
  public function getLocations() {
    $locations = array();
    if (!$this->connected) {
      return $locations;
    }

    $result = $this->db->query('SELECT branch_id, branch_town, branch_address, branch_zip, branch_fax, branch_phone, branch_mail, brand_id, brand_short ' .
      'FROM anm_branches WHERE brand_id > 0 ORDER BY branch_id ASC');
    while ($row = $result->fetch_object()) {
      $location = new StdClass();
      $location->id = $row->branch_id;
      $location->name = $row->branch_town;
      $location->address = $row->branch_address;
      $location->plz = $row->branch_zip;
      $location->fax = $row->branch_fax;
      $location->phone = $row->branch_phone;
      $location->email = $row->branch_mail;
      $location->brand_id = $row->brand_id;
      $location->brand_short = $row->brand_short;
      $locations[] = $location;
    }
    return $locations;
  }

  /**
   * Gets the location (branch) by branch id
   *
   * @url GET /locations/$id
   */
  public function getLocation($id) {
    $location = FALSE;
    if (!$this->connected) {
      return $location;
    }

    $id = intval($id);
    $result = $this->db->query('SELECT branch_id, branch_town, branch_address, branch_zip, branch_fax, branch_phone, branch_mail, brand_id, brand_short ' .
      'FROM anm_branches WHERE brand_id > 0 AND branch_id = ' . $id);
    if ($row = $result->fetch_object()) {
      $location = new StdClass();
      $location->id = $row->branch_id;
      $location->name = $row->branch_town;
      $location->address = $row->branch_address;
      $location->plz = $row->branch_zip;
      $location->fax = $row->branch_fax;
      $location->phone = $row->branch_phone;
      $location->email = $row->branch_mail;
      $location->brand_id = $row->brand_id;
      $location->brand_short = $row->brand_short;
    }
    return $location;
  }
}
