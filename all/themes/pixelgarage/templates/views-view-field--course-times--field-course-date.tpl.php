<?php
//
// set anmelde- or anzeigen-link on date
if (!empty($row->field_field_anmeldungs_link) && !empty($row->field_field_anmeldungs_link[0]['raw']['value'])) {
  // if Anmeldungs link is available, set this link
  $output = l($output, $row->field_field_anmeldungs_link[0]['raw']['value'], array('html' => TRUE));
} else {
  $output = l($output, 'node/' . $row->nid . '/anmeldung', array('html' => TRUE));
}
?>
<?php print $output; ?>
