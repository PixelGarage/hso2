<?php
/**
 * @file
 * Bootstrap 5-7 template for Display Suite.
 */

$account = $variables['elements']['#account'];

?>


<<?php print $layout_wrapper; print $layout_attributes; ?> class="<?php print $classes; ?>">
  <?php if (isset($title_suffix['contextual_links'])): ?>
    <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>
  <div class="row">
    <<?php print $left_wrapper; ?> class="col-profile col-left <?php print $left_classes; ?>">
      <?php print $left; ?>
    </<?php print $left_wrapper; ?>>
    <<?php print $right_wrapper; ?> class="col-profile col-right <?php print $right_classes; ?>">
      <!-- print title and full name-->
      <div class="title-with-full-name">
        <?php print render($user_profile['field_title']); ?>
        <?php print render($user_profile['field_full_name']); ?>
      </div>

      <!-- print position -->
      <?php print render($user_profile['field_position_function']); ?>

      <!-- print bio -->
      <?php print render($user_profile['field_bio']); ?>

      <!-- print phone and email -->
      <?php if (!in_array('dozent', $account->roles) && isset($user_profile['field_phone'])): ?>
        <div class="wrapper-phone">
          <span class="fa fa-phone"></span>
          <?php print render($user_profile['field_phone']); ?>
        </div>
      <?php endif; ?>
      <?php if (isset($account->mail)): ?>
        <div class="wrapper-email">
          <span class="fa fa-envelope"></span>
          <div class="field-email">
            <?php print invisimail_encode_email($account->mail, 'js_entities', array('link' => true)); ?>
          </div>
        </div>
      <?php endif; ?>

      <!-- print place -->
      <?php if (isset($user_profile['field_place'])): ?>
        <div class="wrapper-place">
          <span class="fa fa-map-marker"></span>
          <?php print render($user_profile['field_place']); ?>
        </div>
      <?php endif; ?>

    </<?php print $right_wrapper; ?>>
  </div>
</<?php print $layout_wrapper ?>>


<!-- Needed to activate display suite support on forms -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
