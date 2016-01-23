<?php
/**
 * Created by PhpStorm.
 * User: ralph
 * Date: 06.02.15
 * Time: 17:22
 */

/**
 * Output a form element in plain text format.
 */
function hso_theme_webform_element_text($variables) {
  $element = $variables['element'];
  $value = $variables['element']['#children'];

  $output = '';
  $is_group = webform_component_feature($element['#webform_component']['type'], 'group');

  // Output the element title.
  if (isset($element['#title'])) {
    if ($is_group) {
      $output .= '--' . $element['#title'] . '--';
    }
    elseif (!in_array(drupal_substr($element['#title'], -1), array('?', ':', '!', '%', ';', '@'))) {
      $output .= $element['#title'] . ':';
    }
    else {
      $output .= $element['#title'];
    }
  }
  // Wrap long values at 65 characters, allowing for a few fieldset indents.
  // It's common courtesy to wrap at 75 characters in e-mails.
  if ($is_group && drupal_strlen($value) > 65) {
    $value = wordwrap($value, 65, "\n");
    $lines = explode("\n", $value);
    foreach ($lines as $key => $line) {
      $lines[$key] = '  ' . $line;
    }
    $value = implode("\n", $lines);
  }

  // Add the value to the output.
  if ($value) {
    $output .= (strpos($value, "\n") === FALSE ? ' ' : "\n") . $value;
  }

  // Indent fieldsets.
  if ($is_group) {
    $lines = explode("\n", $output);
    foreach ($lines as $number => $line) {
      if (strlen($line)) {
        $lines[$number] = '  ' . $line;
      }
    }
    $output = implode("\n", $lines);
  }

  if ($output) {
    $output .= "\n";
  }

  return $output;
}

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
