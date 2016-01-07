<?php
/**
 * Created by PhpStorm.
 * User: ralph
 * Date: 06.02.15
 * Time: 17:23
 */

/**
 * Preprocess variables for the html template.
 */
function pixelgarage_preprocess_html(&$vars) {
  // make sure jQuery UI and effects is loaded for anonymous users
  drupal_add_library('system', 'ui');
  drupal_add_library('system', 'effects');

}

/**
 * Override or insert variables for the page templates.
 */
function pixelgarage_preprocess_page(&$vars) {
  // hide titles on login forms
  pg_login_preprocess_page($vars);

  // set segment logo
  $contexts = context_active_contexts();
  if (array_key_exists('kv-college', $contexts)) {
    $vars['logo'] = str_replace('logo.png', 'kv_logo.png', $vars['logo']);
  }
  if (array_key_exists('hs-college', $contexts)) {
    $vars['logo'] = str_replace('logo.png', 'hs_logo.png', $vars['logo']);
  }
  if (array_key_exists('ks-college', $contexts)) {
    $vars['logo'] = str_replace('logo.png', 'ks_logo.png', $vars['logo']);
  }
  if (array_key_exists('ex-college', $contexts)) {
    $vars['logo'] = str_replace('logo.png', 'ex_logo.png', $vars['logo']);
  }
}


