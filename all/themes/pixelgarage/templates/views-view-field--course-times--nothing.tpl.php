<?php
//
// define Anmelde- or Anzeigen-button
if (!empty($row->field_field_anmeldungs_link) && !empty($row->field_field_anmeldungs_link[0]['raw']['value'])) {
  // if Anmeldungs link is available, set this link
  $output = l(t('Anzeigen'), $row->field_field_anmeldungs_link[0]['raw']['value'], array('html' => TRUE, 'attributes' => array('target'=>'_blank')));
} else {
  $output = l($output, 'node/' . $row->nid . '/anmeldung', array('html' => TRUE));
}
?>
<?php print $output; ?>
