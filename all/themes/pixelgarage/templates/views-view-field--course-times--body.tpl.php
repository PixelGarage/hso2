<?php
//
// fill the body field with study variant and teaching times (location dependant)
$course_times = $row->_field_data['nid']['entity'];
$location = $row->_field_data['node_field_data_field_location_nid']['entity'];
$output_prefix = false;

if (isset($course_times->field_study_variant[LANGUAGE_NONE][0]['tid'])) {
  $tid = $course_times->field_study_variant[LANGUAGE_NONE][0]['tid'];
  $study_variant = taxonomy_term_load($tid);
  $output_prefix = $study_variant->name . '<br/>';

  if (isset($location->field_teaching_times[LANGUAGE_NONE])) {
    foreach($location->field_teaching_times[LANGUAGE_NONE] as $index => $teaching_time) {
      $field_coll = field_collection_item_load($teaching_time['value']);
      if ($tid == $field_coll->field_study_variant[LANGUAGE_NONE][0]['tid']) {
        $output_prefix .= isset($field_coll->field_variant_teaching_times[LANGUAGE_NONE][0]['safe_value']) ?
          $field_coll->field_variant_teaching_times[LANGUAGE_NONE][0]['safe_value'] . '<br/>' : '';
      }
    }
  }
}
?>
<?php print $output_prefix ? $output_prefix . $output : $output; ?>
