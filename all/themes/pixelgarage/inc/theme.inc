<?php
/**
 * Created by PhpStorm.
 * User: ralph
 * Date: 06.02.15
 * Time: 17:22
 */

/**
 * Field theme specific override for body field in a remote form.
 *
 * Adds the profile id to the iFrame src attribute for the Kundenprofil remote form.
 *
 * @param $variables
 *
 * @return string
 */
function pixelgarage_field__body__remote_form ($variables) {
  // adapt the body field for Kundenprofil
  $content = $variables['items'][0]['#markup'];
  if (strpos($content, 'if-kundenprofil') > 0) {
    // replace delivered profile id in content
    $profile_id =  (array_key_exists('id', $_GET)) ? $_GET['id'] : '';
    $variables['items'][0]['#markup'] = str_replace('%cid', $profile_id, $content);
  }

  // render the body field in the remote form content type
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<div class="field-label"' . $variables['title_attributes'] . '>' . $variables['label'] . ':&nbsp;</div>';
  }

  // Render the items.
  $output .= '<div class="field-items"' . $variables['content_attributes'] . '>';
  foreach ($variables['items'] as $delta => $item) {
    $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
    $output .= '<div class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</div>';
  }
  $output .= '</div>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}
