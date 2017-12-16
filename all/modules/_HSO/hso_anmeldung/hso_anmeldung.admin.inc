<?php
/**
 * Created by PhpStorm.
 * User: ralph
 * Date: 11.10.16
 * Time: 23:15
 */

function hso_anmeldung_prices_form($form, &$form_state) {
  $form = array();

  // configure the price set for TELC B1
  $form['hso_anmeldung_telc_a'] = array(
    '#type' => 'fieldset',
    '#title' => t('TELC A exam prices'),
    '#description' => t('Configure the prices for TELC A exams.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['hso_anmeldung_telc_a']['hso_anmeldung_telca1'] = array(
    '#type' => 'textfield',
    '#title' => t('A1'),
    '#default_value' => variable_get('hso_anmeldung_telca1', 0),
    '#size' => 60,
    '#field_suffix' => t('CHF'),
    '#required' => TRUE,
  );
  $form['hso_anmeldung_telc_a']['hso_anmeldung_telca2'] = array(
    '#type' => 'textfield',
    '#title' => t('A2'),
    '#default_value' => variable_get('hso_anmeldung_telca2', 0),
    '#size' => 60,
    '#field_suffix' => t('CHF'),
    '#required' => TRUE,
  );

  // configure the price set for TELC B1
  $form['hso_anmeldung_telc_b1'] = array(
    '#type' => 'fieldset',
    '#title' => t('TELC B1 exam prices'),
    '#description' => t('Configure the prices for TELC B1 exams.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['hso_anmeldung_telc_b1']['hso_anmeldung_oral_exam_telcb1'] = array(
    '#type' => 'textfield',
    '#title' => t('Oral'),
    '#default_value' => variable_get('hso_anmeldung_oral_exam_telcb1', 0),
    '#description' => t('Set the price for an oral exam.'),
    '#size' => 60,
    '#field_suffix' => t('CHF'),
    '#required' => TRUE,
  );
  $form['hso_anmeldung_telc_b1']['hso_anmeldung_written_exam_telcb1'] = array(
    '#type' => 'textfield',
    '#title' => t('Written'),
    '#default_value' => variable_get('hso_anmeldung_written_exam_telcb1', 0),
    '#description' => t('Set the price for a written exam.'),
    '#size' => 60,
    '#field_suffix' => t('CHF'),
    '#required' => TRUE,
  );
  $form['hso_anmeldung_telc_b1']['hso_anmeldung_both_exam_telcb1'] = array(
    '#type' => 'textfield',
    '#title' => t('Oral and written'),
    '#default_value' => variable_get('hso_anmeldung_both_exam_telcb1', 0),
    '#description' => t('Set the price for both oral and written exam.'),
    '#size' => 60,
    '#field_suffix' => t('CHF'),
    '#required' => TRUE,
  );

  // configure the price set for TELC B2
  $form['hso_anmeldung_telc_b2'] = array(
    '#type' => 'fieldset',
    '#title' => t('TELC B2 exam prices'),
    '#description' => t('Configure the prices for TELC B2 exams.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['hso_anmeldung_telc_b2']['hso_anmeldung_oral_exam_telcb2'] = array(
    '#type' => 'textfield',
    '#title' => t('Oral'),
    '#default_value' => variable_get('hso_anmeldung_oral_exam_telcb2', 0),
    '#description' => t('Set the price for an oral exam.'),
    '#size' => 60,
    '#field_suffix' => t('CHF'),
    '#required' => TRUE,
  );
  $form['hso_anmeldung_telc_b2']['hso_anmeldung_written_exam_telcb2'] = array(
    '#type' => 'textfield',
    '#title' => t('Written'),
    '#default_value' => variable_get('hso_anmeldung_written_exam_telcb2', 0),
    '#description' => t('Set the price for a written exam.'),
    '#size' => 60,
    '#field_suffix' => t('CHF'),
    '#required' => TRUE,
  );
  $form['hso_anmeldung_telc_b2']['hso_anmeldung_both_exam_telcb2'] = array(
    '#type' => 'textfield',
    '#title' => t('Oral and written'),
    '#default_value' => variable_get('hso_anmeldung_both_exam_telcb2', 0),
    '#description' => t('Set the price for both oral and written exam.'),
    '#size' => 60,
    '#field_suffix' => t('CHF'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

