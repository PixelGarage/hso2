<?php
/**
 * @file
 * Contains administrator's settings form and validators
 */

/**
 * Settings page form callback.
 */
function flexpaper2_settings_page_form($form, &$form_state) {
  $form['flexpaper2_path_to_pdf2swf'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to pdf2swf'),
    '#description' => t('For example /usr/local/bin/pdf2swf'),
    '#default_value' => variable_get('flexpaper2_path_to_pdf2swf', 'pdf2swf'),
    '#size' => 100,
    '#maxlength' => 300,
    '#required' => TRUE,
  );
  $form['flexpaper2_path_to_swfrender'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to swfrender'),
    '#description' => t('For example /usr/local/bin/swfrender'),
    '#default_value' => variable_get('flexpaper2_path_to_swfrender', 'swfrender'),
    '#size' => 100,
    '#maxlength' => 300,
    '#required' => TRUE,
  );
  $form['flexpaper2_path_to_pdf2json'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to pdf2json'),
    '#description' => t('For example /usr/local/bin/pdf2json'),
    '#default_value' => variable_get('flexpaper2_path_to_pdf2json', 'pdf2json'),
    '#size' => 100,
    '#maxlength' => 300,
    '#required' => TRUE,
  );
  $form['flexpaper2_path_to_pdftk'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to pdftk'),
    '#description' => t('For example /usr/local/bin/pdftk'),
    '#default_value' => variable_get('flexpaper2_path_to_pdftk', 'pdftk'),
    '#size' => 100,
    '#maxlength' => 300,
    '#required' => TRUE,
  );
  $form['flexpaper2_license_key'] = array(
    '#type' => 'textfield',
    '#title' => t('License key'),
    '#description' => t('License key for the commercial version of the FlexPaper viewer.'),
    '#default_value' => variable_get('flexpaper2_license_key', ''),
    '#size' => 100,
    '#maxlength' => 300,
  );
  return system_settings_form($form);
}

/**
 * Validation handler for settings page form.
 */
function flexpaper2_settings_page_form_validate($form, &$form_state) {
  return;
  $path_to_pdf2swf = $form_state['values']['flexpaper2_path_to_pdf2swf'];
  $path_to_swfrender = $form_state['values']['flexpaper2_path_to_swfrender'];
  $path_to_pdf2json = $form_state['values']['flexpaper2_path_to_pdf2json'];
  $path_to_pdftk = $form_state['values']['flexpaper2_path_to_pdftk'];
  if (!flexpaper2_cmdtool_available($path_to_pdf2swf, 'pdf2swf')) {
    form_set_error('flexpaper2_path_to_pdf2swf', t('Path to pdf2swf tool is not properly set.'));
  }
  if (!flexpaper2_cmdtool_available($path_to_swfrender, 'swfrender')) {
    form_set_error('flexpaper2_path_to_swfrender', t('Path to swfrender tool is not properly set.'));
  }
  if (!flexpaper2_cmdtool_available($path_to_pdf2json, 'pdf2json')) {
    form_set_error('flexpaper2_path_to_pdf2json', t('Path to pdf2json tool is not properly set.'));
  }
  if (!flexpaper2_cmdtool_available($path_to_pdftk, 'pdftk')) {
    form_set_error('flexpaper2_path_to_pdftk', t('Path to pdftk tool is not properly set.'));
  }
}

