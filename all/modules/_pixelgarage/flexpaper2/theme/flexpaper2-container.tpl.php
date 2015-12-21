<?php

/**
 * @file
 * Theme implementation for displaying flexpaper formatter.
 */
?>
<div class="flexpaper-container">
  <div class="flexpaper-header">
    <select class="<?php print $select_classes; ?>">
      <?php foreach($options as $option): ?>
        <?php print render($option); ?>
      <?php endforeach; ?>
    </select>
    <div class="flexpaper-link-container"></div>
  </div>

  <!-- Do not change following element structure, it will be overridden by Flexpaper -->
  <div class="flexpaper_viewer"><?php print $flexpaper_info; ?></div>
</div>
