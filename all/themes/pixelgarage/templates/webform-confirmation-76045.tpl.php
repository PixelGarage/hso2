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
 * - $progressbar: The progress bar 100% filled (if configured). This may not
 *   print out anything if a progress bar is not enabled for this node.
 * - $confirmation_message: The confirmation message input by the webform
 *   author.
 * - $sid: The unique submission ID of this submission.
 * - $url: The URL of the form (or for in-block confirmations, the same page).
 */
?>
<?php print $progressbar; ?>

<div class="webform-confirmation">
  <?php if ($confirmation_message): ?>
    <?php print $confirmation_message ?>
  <?php else: ?>
    <p><?php print t('Herzlichen Dank für Ihre Reservierung. Sie erhalten in Kürze eine Bestätigung per Mail.'); ?></p><br>
    <p><?php print t('Bei Fragen stehen wir Ihnen gerne zur Verfügung.'); ?><br><?php print t('Ihr HSO-Team'); ?></p><br>
    <p><?php print t('058 680 14 00'); ?></p>
  <?php endif; ?>
</div>
