<?php
/**
 * Created by PhpStorm.
 * User: ralph
 * Date: 02.08.17
 * Time: 17:15
 */

/**
 * Alter the node form for course-times.
 *
 * @param $form
 * @param $form_state
 */
function hso_mods_form_node_form_alter(&$form, &$form_state) {
  // adapt only course times node forms
  if ($form['#bundle'] <> 'course_times') return;

  //
  // extend course_times nodes with generator fields and master flag
  $master_flag = isset($form_state['values']['master_flag']) ? $form_state['values']['master_flag'] : 0;
  $master_nid = $form['#node']->nid;

  $form['master_flag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable course-time generator'),
    '#default_value' => $master_flag,
    '#description' => t('Enable flag to access node generator settings. A generator node is only used to generate other nodes according to the settings.'),
    '#ajax' => array(
      'callback' => 'node_generator_ajax_callback',
      'wrapper' => 'node-generator-settings',
      'method' => 'replace',
    ),
    '#weight' => 0,
  );
  $form['master_wrapper'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="node-generator-settings"></div>',
    '#weight' => 1,
  );

  // add "Delete Generated Nodes" button, if generated nodes exist
  if (_get_generated_node_nids($master_nid, true)) {
    $form['actions']['delete_nodes'] = array(
      '#type' => 'submit',
      '#value' => t('Delete Generated Nodes'),
      '#weight' => 20,
      '#submit' => array('node_submit_delete_nodes'),
    );
  }

  // update form submission button
  $form['actions']['submit']['#value'] = t('Save & Generate');
  array_push($form['actions']['submit']['#submit'], 'node_submit_generate_nodes');

  if (!$master_flag) return;

  //
  // display generator settings
  $form['generator_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Course-Time generator'),
    '#collapsible' => true,
    '#collapsed' => false,
    '#description' => t('Define the settings to generate the course-time nodes. For each selection combination a new course-time node is generated. All set fields in this master node are considered by the generation.'),
    '#prefix' => '<div id="node-generator-settings">',
    '#suffix' => '</div>',
    '#weight' => 1,
  );
  $form['generator_settings']['title_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Title pattern'),
    '#default_value' => t('@course @@location - @time'),
    '#size' => 100,
    '#description' => t('Define the pattern for the title generation. Available tokens are: @location, @course, @time'),
  );
  $form['generator_settings']['start_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Start date (Monday)'),
    '#description' => t('Define the study start date. Set always a Monday as start date. The start dates of all other selected study variants are calculated from this date.'),
  );

  $locations = _get_locations();
  $form['generator_settings']['locations'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select locations'),
    '#options' => $locations,
    '#description' => t('Select all locations that have to be generated.'),
  );

  $variants = _get_study_variants();
  $form['generator_settings']['study_variants'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select study variants'),
    '#options' => $variants,
    '#description' => t('Select all study variants that have to be generated.'),
  );

}

/**
 * Ajax callback for master flag.
 */
function node_generator_ajax_callback($form, &$form_state) {
  // return wrapper according to master flag (with settings or empty)
  $master_flag = isset($form_state['values']['master_flag']) ? $form_state['values']['master_flag'] : 0;
  return $master_flag ? $form['generator_settings'] : $form['master_wrapper'];
}

/**
 * Node generator for course-time nodes.
 *
 * @param $form
 * @param $form_state
 */
function node_submit_generate_nodes($form, &$form_state) {
  //
  // only generate course-time nodes when master flag is set
  $master_flag = isset($form_state['values']['master_flag']) ? $form_state['values']['master_flag'] : 0;
  if (!$master_flag) return;

  //
  // save master as unpublished (never show in frontend)
  $master_nid = $form_state['values']['nid'];
  $master_node = node_load($master_nid);
  $master_node->status = 0;
  node_save($master_node);

  //
  // generate course-times nodes for all selected locations and study-variants
  $course = node_load($form_state['values']['field_course'][LANGUAGE_NONE][0]['target_id']);
  $locations = $form_state['values']['locations'];
  $study_variants = $form_state['values']['study_variants'];
  $title_pattern = $form_state['values']['title_pattern'];
  $start_timestamp = strtotime($form_state['values']['start_date']); // Monday

  //
  // iterate through all selections
  foreach ($locations as $loc_nid => $loc_selected) {
    if ($loc_selected) {
      $location = node_load($loc_nid);

      // iterate study variants
      foreach ($study_variants as $variant_tid => $var_selected) {
        if ($var_selected) {
          $study_variant = taxonomy_term_load($variant_tid);
          $variant_name = $study_variant->name;
          $timestamp = _get_start_timestamp($variant_name, $start_timestamp);
          $start_date_display = date('d.m.Y', $timestamp);
          $start_date = date('Y-m-d H:i:s', $timestamp);

          //
          // create node
          $course_times = _create_course_times_node($form_state['values']);
          $course_times->title = t($title_pattern, array(
            '@course' => $course->title,
            '@location' => $location->title,
            '@time' => $start_date_display
          ));
          $course_times->field_location[LANGUAGE_NONE][0]['target_id'] = $loc_nid;
          $course_times->field_study_variants[LANGUAGE_NONE][0]['target_id'] = $variant_tid;
          $course_times->field_course_date[LANGUAGE_NONE][0]['value'] = $start_date;
          $course_times->field_master_nid[LANGUAGE_NONE][0]['value'] = $master_nid;
          node_save($course_times);
        }
      }
    }
  }

}

/**
 * Deletes all nodes generated by the master node.
 */
function node_submit_delete_nodes($form, &$form_state) {
  $master_nid = $form['#node']->nid;
  if ($nids = _get_generated_node_nids($master_nid)) {
    node_delete_multiple($nids);
  }
}



/* --------------------------------------------------
 *    Form helper functions
 * --------------------------------------------------*/
/**
 * Creates and returns a new course-times node filled with the master node fields.
 */
function _create_course_times_node($master_values) {
  //
  // create a new course_times node
  $node = new stdClass();
  $node->type = 'course_times';
  $node->language = LANGUAGE_NONE;
  $node->uid = 1;
  $node->status = 1;
  node_object_prepare($node);


  // update all master fields of node
  $node->field_course[LANGUAGE_NONE][0]['target_id'] = $master_values['field_course'][LANGUAGE_NONE][0]['target_id'];
  $node->field_has_fsv[LANGUAGE_NONE][0]['value'] = $master_values['field_has_fsv'][LANGUAGE_NONE][0]['value'];
  $node->field_brutto_price[LANGUAGE_NONE][0]['value'] = $master_values['field_brutto_price'][LANGUAGE_NONE][0]['value'];
  $node->field_netto_price[LANGUAGE_NONE][0]['value'] = $master_values['field_netto_price'][LANGUAGE_NONE][0]['value'];
  $node->field_detailed_price[LANGUAGE_NONE][0]['value'] = $master_values['field_detailed_price'][LANGUAGE_NONE][0]['value'];
  $node->field_detailed_price[LANGUAGE_NONE][0]['format']  = 'html';
  $node->field_template[LANGUAGE_NONE][0]['value'] = $master_values['field_template'][LANGUAGE_NONE][0]['value'];
  $node = node_submit($node);
  return $node;
}

function _get_start_timestamp($variant_name, $initial_timestamp) {
  // return timestamp according to selected study variant
  $variant_name = strtoupper($variant_name);
  if (strpos($variant_name, 'MO') !== false) {
    return $initial_timestamp;
  }
  else if (strpos($variant_name, 'DI') !== false) {
    $timestamp = strtotime('+ 1 day', $initial_timestamp);
    return $timestamp;
  }
  else if (strpos($variant_name, 'MI') !== false) {
    $timestamp = strtotime('+ 2 day', $initial_timestamp);
    return $timestamp;
  }
  else if (strpos($variant_name, 'DO') !== false) {
    $timestamp = strtotime('+ 3 day', $initial_timestamp);
    return $timestamp;
  }
  else if (strpos($variant_name, 'FR') !== false) {
    $timestamp = strtotime('+ 4 day', $initial_timestamp);
    return $timestamp;
  }
  else if (strpos($variant_name, 'SA') !== false) {
    $timestamp = strtotime('+ 5 day', $initial_timestamp);
    return $timestamp;
  }
}


function _get_study_variants() {
  $options = array();
  $query = new EntityFieldQuery();
  $result = $query
    ->entityCondition('entity_type', 'taxonomy_term')
    ->entityCondition('bundle', 'study_variants')
    ->propertyOrderBy('weight')
    ->execute();

  if (isset($result['taxonomy_term'])) {
    $tids = array_keys($result['taxonomy_term']);
    foreach ($tids as $tid) {
      $term = taxonomy_term_load($tid);
      $options[$term->tid] = $term->name;
    }
  }

  return $options;
}

function _get_locations() {
  $options = array();
  $query = new EntityFieldQuery();
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'location')
    ->propertyOrderBy('title')
    ->execute();

  if (isset($result['node'])) {
    $nodes = node_load_multiple(array_keys($result['node']));
    foreach ($nodes as $node) {
      $options[$node->nid] = $node->title;
    }
  }

  return $options;
}

function _get_generated_node_nids($master_nid, $count_only = false) {
  $return = $count_only ? 0 : array();
  $query = new EntityFieldQuery();
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'course_times')
    ->fieldCondition('field_master_nid', 'value', $master_nid)
    ->execute();

  if (isset($result['node'])) {
    $nids = array_keys($result['node']);
    $return = $count_only ? count($nids) : $nids;
  }
  return $return;
}

