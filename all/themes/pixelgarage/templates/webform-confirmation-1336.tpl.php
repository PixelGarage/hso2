<?php
/**
 * @file
 * Customize confirmation screen after successful submission.
 *
 * This file may be renamed "webform-confirmation-[nid].tpl.php" to target a
 * specific webform e-mail on your site. Or you can leave it
 * "webform-confirmation.tpl.php" to affect all webform confirmations on your
 * site.
 *
 * Available variables:
 * - $node: The node object for this webform.
 * - $confirmation_message: The confirmation message input by the webform author.
 * - $sid: The unique submission ID of this submission.
 */

module_load_include('inc', 'webform', 'includes/webform.submissions');
$submission = webform_get_submission($node->nid, $sid);
$brochure = null;

// get course node
$webform_components = $node->webform['components'];
foreach ($webform_components as $key => $data) {
  if ($data['form_key'] == 'lehrgang') {
    $course_nid = $submission->data[$key][0];
    break;
  }
}

if ($course_nid) {
  $course = node_load($course_nid);
  $segment = !empty($course->field_segment) ? taxonomy_term_load($course->field_segment[LANGUAGE_NONE][0]['tid']) : FALSE;
  if ($course && !empty($course->field_brochure)) {
    $brochure = node_load($course->field_brochure[LANGUAGE_NONE][0]['target_id']);
    if ($brochure && !empty($brochure->field_file)) {
      // render brochure (flexpaper viewer)
      $viewer = node_view($brochure);

      //$pdf = $brochure->field_file[LANGUAGE_NONE][0]['uri'];
      //hso_anmeldung_transfer_pdf($pdf, 'Kurs-Details.pdf', false);
    }
  }
}
?>

<div class="webform-confirmation">
  <?php if ($confirmation_message): ?>
    <?php print $confirmation_message ?>
  <?php else: ?>
    <p>Besten Dank für Ihr Interesse an unseren Ausbildungs-Lehrgängen. Der angeforderte Prospekt ist zur Zeit leider nicht verfügbar.<p>
  <?php endif; ?>
</div>

<div class="brochure-viewer">
  <?php print render($viewer) ?>
</div>

<div class="links">
  <a href="<?php print url('node/' . $course_nid); ?>">Zurück zum Kurs/Lehrgang</a>
</div>

<?php if ($brochure): ?>
  <script type="text/javascript">
    // check existance (opt-out doesn't create ga)
    if (typeof ga == 'function') {
      // Load the ecommerce plug-in.
      ga('require', 'ecommerce');

      // add transaction
      ga('ecommerce:addTransaction', {
        'id': '<?php print $sid; ?>-<?php print $submission->remote_addr; ?>',    // Transaction ID. Required
        'affiliation': 'HSO Download Center',                                     // Affiliation or store name
        'revenue': '0.00',                                                        // Grand Total
        'shipping': '0',                                                          // Shipping
        'tax': '0.0'                                                              // Tax
      });

      // add ecommerce item
      ga('ecommerce:addItem', {
        'id': '<?php print $sid; ?>-<?php print $submission->remote_addr; ?>',    // Transaction ID. Required
        'name': '<?php print addslashes($brochure->title); ?>',                   // Product name. Required
        'sku': '<?php print $brochure->field_sku_code[LANGUAGE_NONE][0]['value']; ?>', // SKU/code
        'category': '<?php print addslashes($segment->name); ?>',                 // Category or variation
        'price': 'no',                                                            // Unit price
        'quantity': '1'                                                           // Quantity
      });

      // submit transaction
      ga('ecommerce:send');      // Send transaction and item data to Google Analytics.
    }
  </script>
<?php endif; ?>


