<?php
module_load_include('inc', 'webform', 'includes/webform.submissions');
$submission = webform_get_submission($node->nid, $sid);

// get course time node
$course_time_nid = null;
$level = $level_b = '';
$exam_type = '';
$webform_components = $node->webform['components'];
foreach ($webform_components as $key => $data) {
	if ($data['form_key'] == 'course_times_nid') {
		$course_time_nid = $submission->data[$key][0];
	}
  if ($data['form_key'] == 'level_a') {
    $level = $submission->data[$key][0];
  }
  if ($data['form_key'] == 'level_b') {
    $level_b = $submission->data[$key][0];
  }
  if ($data['form_key'] == 'pruefungsart') {
    $exam_type = $submission->data[$key][0];
  }
}
if ($course_time_nid) {
	$tracking_price = 0;
	$course_time_node = node_load($course_time_nid);
	if ($course_time_node && !empty($course_time_node->field_course)) {
	  $sku = 'df_' . (!empty($course_time_node->field_internal_id) ? $course_time_node->field_internal_id[LANGUAGE_NONE][0]['value'] : 'XXXX');
    $course_time_description = !empty($course_time_node->body) ? strip_tags($course_time_node->body[LANGUAGE_NONE][0]['value']) : '';
    $course = node_load($course_time_node->field_course[LANGUAGE_NONE][0]['target_id']);
    $item_name = $course->title . (empty($course_time_description) ? '' : ', ' . $course_time_description);
		$segment = !empty($course->field_segment) ? taxonomy_term_load($course->field_segment[LANGUAGE_NONE][0]['tid']) : FALSE;
		$location = !empty($course_time_node->field_location) ? node_load($course_time_node->field_location[LANGUAGE_NONE][0]['target_id']) : FALSE;
		$pdf_link = url('get_anmeldung/' . md5($sid . '2I7L1N1'));
    switch ($exam_type) {
      case 'muendlich':
        $type = 'hso_anmeldung_oral_exam_telc';
        $level = $level_b;
        break;
      case 'schriftlich':
        $type = 'hso_anmeldung_written_exam_telc';
        $level = $level_b;
        break;
      case 'beides':
        $type = 'hso_anmeldung_both_exam_telc';
        $level = $level_b;
        break;
      default:
        $type = 'hso_anmeldung_telc';
    }
    $tracking_price = variable_get($type . $level, 0);
	}
}
?>
<div class="webform-confirmation">
  <p>Besten Dank für Ihre Prüfungs-Anmeldung.<p>
  <ul>
    <li>Die <a target="_blank" href="<?php print $pdf_link; ?>">Anmeldebestätigung (PDF)</a> können Sie nun herunterladen</li>
    <li>Per E-Mail erhalten Sie dieselbe Anmeldebestätigung in einigen Minuten zugestellt</li>
  </ul>

  <h2>Weiteres Vorgehen</h2>
  <p>Beachten Sie auf der Liste bitte unbedingt die Anmeldeschl&uuml;sse f&uuml;r TELC-Pr&uuml;fungen (jeweils ca. 5 Wochen vor dem jeweiligen Pr&uuml;fungstermin).
    Falls Sie sich nach dem Anmeldeschluss anmelden wird Ihnen einen Zuschlag von CHF 50.– f&uuml;r die Stufen A1-A2, oder CHF 75.– für die Stufen B1-B2 verrechnet."</p>
  <p>Die Pr&uuml;fung kann nur durchef&uuml;hrt werden, wenn sich daf&uuml;r gen&uuml;gend Interessent/-innen finden.
  Falls die Pr&uuml;fung nicht durchgef&uuml;hrt werden kann, verst&auml;ndigen wir Sie nach dem Anmeldeschluss.</p>

  <h2>Rückgängig machen</h2>
  <p>Wollen Sie Ihre Anmeldung rückgängig machen, nehmen Sie bitte telefonisch mit uns
  	Kontakt auf.</p>

  <p><a href="<?php print url('node/' . $course->nid); ?>">Zurück zur Sprachschule</a></p>
</div>

<?php if ($course_time_nid): ?>
  <script type="text/javascript">
    // check existance (opt-out doesn't create ga)
    if (typeof ga == 'function') {
      // Load the ecommerce plug-in.
      ga('require', 'ecommerce');

      // add transaction
      ga('ecommerce:addTransaction', {
        'id': '<?php print $sid; ?>-<?php print $submission->remote_addr; ?>',    // Transaction ID. Required
        'affiliation': 'HSO <?php print addslashes($location->title); ?>',        // Affiliation or store name
        'revenue': '<?php print $tracking_price; ?>',                             // Grand Total
        'shipping': '0',                                                          // Shipping
        'tax': '0.0'                                                              // Tax
      });

      // add ecommerce item
      ga('ecommerce:addItem', {
        'id': '<?php print $sid; ?>-<?php print $submission->remote_addr; ?>',    // Transaction ID. Required
        'name': '<?php print addslashes($item_name); ?>',                         // Product name. Required
        'sku': '<?php print $sku; ?>', // SKU/code
        'category': '<?php print addslashes($segment->name); ?>',                 // Category or variation
        'price': '<?php print $tracking_price; ?>',                               // Unit price
        'quantity': '1'                                                           // Quantity
      });

      // submit transaction
      ga('ecommerce:send');      // Send transaction and item data to Google Analytics.
    }
  </script>
<?php endif; ?>
