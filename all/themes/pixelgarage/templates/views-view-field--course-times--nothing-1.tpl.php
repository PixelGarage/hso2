<?php
//
// show Reservation-button until 10 days before course start date
$now = time();
$course_times = $row->_field_data['nid']['entity'];
$course = node_load($course_times->field_course[LANGUAGE_NONE][0]['target_id']);
$segment = !empty($course->field_segment) ? $course->field_segment[LANGUAGE_NONE][0]['tid'] : 0;
$start_date = $course_times->field_course_date[LANGUAGE_NONE][0]['value'];
$start_timestamp = strtotime($start_date);
$output = (($start_timestamp - $now) > 864000 && $segment != 160) ? l($output, 'node/' . $row->nid . '/reservation', array('html' => TRUE)) : '';
?>
<?php print $output; ?>
