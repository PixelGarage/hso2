<?php
/**
 * @file
 * Bootstrap 12 template for Display Suite.
 */

$address = $node->field_address[LANGUAGE_NONE][0];
$map_location = urlencode($address['thoroughfare'] . ', ' . $address['postal_code'] . ' ' . $address['locality']);

?>


<<?php print $layout_wrapper; print $layout_attributes; ?> class="<?php print $classes; ?>">
  <?php if (isset($title_suffix['contextual_links'])): ?>
    <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>
  <div class="row">
    <<?php print $central_wrapper; ?> class="col-sm-12 <?php print $central_classes; ?>">
      <div class="col-left">
        <!-- print picture and address block -->
        <div class="picture-address">
          <?php print render($content['field_picture']); ?>
          <div class="title-address">
            <?php if ($title): ?>
              <?php print render($title_prefix); ?>
              <header class="title-location">
                <h2<?php print $title_attributes; ?>><?php print $title ?></h2>
              </header>
              <?php print render($title_suffix); ?>
            <?php endif; ?>
            <?php print render($content['field_address']); ?>
          </div>
        </div>

        <!-- print location pin -->
        <div class="map-location-pin">
          <a href="https://maps.google.com/maps?q=<?php print $map_location; ?>" target="_blank">Map</a>
        </div>

        <!-- print body -->
        <div class="body">
          <?php print render($content['body']); ?>
        </div>
      </div>

      <div class="col-right">
      <!-- print location map -->
        <div class="map-location">
          <a href="https://maps.google.com/maps?q=<?php print $map_location; ?>" target="_blank">
            <img src="https://maps.googleapis.com/maps/api/staticmap?center=<?php print $map_location; ?>&language=de&size=275x190&maptype=roadmap&markers=color:red%7C<?php print $map_location; ?>&sensor=false" alt="" />
          </a>
        </div>

        <!-- print map details -->
        <div class="map-details">
          <?php print render($content['field_map_details']); ?>
        </div>
      </div>

    </<?php print $central_wrapper; ?>>
  </div>
</<?php print $layout_wrapper ?>>


<!-- Needed to activate display suite support on forms -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
