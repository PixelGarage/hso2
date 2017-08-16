<?php
//
// show Reservation-button until 10 days before course start date
$now = time();
$course_times = $row->_field_data['nid']['entity'];
$start_date = $course_times->field_course_date[LANGUAGE_NONE][0]['value'];
$start_timestamp = strtotime($start_date);
$output = (($start_timestamp - $now) > 864000) ? l($output, 'node/' . $row->nid . '/reservation', array('html' => TRUE)) : '';
?>
<?php print $output; ?>
