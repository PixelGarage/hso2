<?php
/**
 * @file
 * Bootstrap 12 template for Display Suite.
 */

$screen_name = $field_screen_name[0]['value'];
$twitter_id = (int)$field_twitter_id[0]['value'];

$reply = t('Reply');
$retweet = t('Retweet');
$favorite = t('Favorite');

?>


<<?php print $layout_wrapper; print $layout_attributes; ?> class="<?php print $classes; ?>">
  <?php if (isset($title_suffix['contextual_links'])): ?>
    <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>
  <div class="row">
    <<?php print $central_wrapper; ?> class="col-sm-12 <?php print $central_classes; ?>">

      <div class="avatar">
        <a href="https://twitter.com/<?php print $screen_name; ?>" title="<?php print $screen_name; ?>" target="_blank">
          <?php print render($content['field_profile_image']); ?>
        </a>
      </div>
      <?php if ($title): ?>
        <?php print render($title_prefix); ?>
        <header class="title-tweet">
          <h2<?php print $title_attributes; ?>><?php print $title ?></h2>
        </header>
        <?php print render($title_suffix); ?>
      <?php endif; ?>

      <!-- Print creation date -->
      <?php print render($content['field_created_time']); ?>

      <div class="screen-name">
        <a href="https://twitter.com/<?php print $screen_name; ?>" target="_blank">@<?php print $screen_name; ?></a>
      </div>

      <!-- Print tweet text -->
      <?php print render($content['body']); ?>

      <ul class="actions">
        <li><a href=
               "https://twitter.com/intent/tweet?in_reply_to=<?php print $twitter_id; ?>" target="_blank"><?php print $reply; ?></a></li>

        <li><a href=
               "https://twitter.com/intent/retweet?tweet_id=<?php print $twitter_id; ?>" target="_blank"><?php print $retweet; ?></a></li>

        <li><a href=
               "https://twitter.com/intent/favorite?tweet_id=<?php print $twitter_id; ?>" target="_blank"><?php print $favorite; ?></a></li>
      </ul>

    </<?php print $central_wrapper; ?>>
  </div>
</<?php print $layout_wrapper ?>>


<!-- Needed to activate display suite support on forms -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
