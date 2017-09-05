<?php
/**
 * Created by PhpStorm.
 * User: ralph
 * Date: 06.02.15
 * Time: 17:23
 */

/* =============================================================================
 *      HTML, page processing
 * ========================================================================== */
/**
 * Preprocess variables for the html template.
 */
function pixelgarage_preprocess_html(&$vars) {
  // set language to german
  $vars['html_attributes_array']['lang'] = 'de';

  // make sure jQuery UI and effects is loaded for anonymous users
  drupal_add_library('system', 'ui');
  drupal_add_library('system', 'effects');

  //
  // add adwebster pixel on kvcollege and handelsschule
  $ad_webster = false;
  if (in_array('kvcollege_section', $vars['classes_array'])) {
    $ad_webster = <<<AdWebster
<!-- Begin ADWEBSTER.COM -->
<!-- img src="http://targeting.adwebster.com/img/bh.gif?n=1301&g=20&a=2267&s=1&l=1&t=i&f=1" width="1" height="1" border="0" -->
<img src="https://secure.adwebster.com/img/bh.gif?n=1301&g=20&a=2267&s=1&l=1&t=i&f=1" width="1" height="1" border="0" >
<!-- end ADWEBSTER.COM   -->
AdWebster;

  } else if (in_array('hscollege_section', $vars['classes_array'])) {
    $ad_webster = <<<AdWebster
<!-- Begin ADWEBSTER.COM -->
<!--img src="http://targeting.adwebster.com/img/bh.gif?n=1301&g=20&a=2266&s=1&l=1&t=i&f=1" width="1" height="1" border="0" -->
<img src="https://secure.adwebster.com/img/bh.gif?n=1301&g=20&a=2266&s=1&l=1&t=i&f=1" width="1" height="1" border="0" >
<!-- end ADWEBSTER.COM   -->
AdWebster;
  }
  if ($ad_webster) {
    $vars['page']['page_bottom']['ad_webster'] = array(
      '#type' => 'markup',
      '#weight' => -100,
      '#markup' => $ad_webster
    );
  }

  //
  // add facebook script on front page
  if (drupal_is_front_page()) {
    $fb_js = <<<FS_JS
<div id="fb-root"></div>
<script>(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) return;
  js = d.createElement(s); js.id = id;
  js.src = "//connect.facebook.net/de_DE/all.js#xfbml=1&appId=100486423397270";
  fjs.parentNode.insertBefore(js, fjs);
}(document, 'script', 'facebook-jssdk'));</script>
FS_JS;
    $vars['page']['page_bottom']['fb_scripts'] = array(
      '#type' => 'markup',
      '#weight' => -100,
      '#markup' => $fb_js
    );
  }

  //
  // add remarketing code to all pages
  $js_js = <<<JS_JS
<script type="text/javascript">
/* <![CDATA[ */
var google_conversion_id = 1045515944;
var google_conversion_label = "rEjgCMi2tAIQqJ3F8gM";
var google_custom_params = window.google_tag_params;
var google_remarketing_only = true;
/* ]]> */
</script>
<script type="text/javascript" src="//www.googleadservices.com/pagead/conversion.js">
</script>
<noscript>
<div style="display:inline;">
<img height="1" width="1" style="border-style:none;" alt="" src="//googleads.g.doubleclick.net/pagead/viewthroughconversion/1045515944/?value=0&amp;label=rEjgCMi2tAIQqJ3F8gM&amp;guid=ON&amp;script=0"/>
</div>
</noscript>
JS_JS;
  $vars['page']['page_bottom']['cx_scripts'] = array(
    '#type' => 'markup',
    '#weight' => -100,
    '#markup' => $js_js
  );
}

/**
 * Override or insert variables for the page templates.
 */
function pixelgarage_preprocess_page(&$vars) {
  // hide titles on login forms
  pg_login_preprocess_page($vars);

  //
  // set segment logo
  $contexts = context_active_contexts();
  if (array_key_exists('kv-college', $contexts) || array_key_exists('kv-college-home', $contexts)) {
    $vars['logo'] = str_replace('logo.png', 'kv_logo.svg', $vars['logo']);
  }
  if (array_key_exists('hs-college', $contexts) || array_key_exists('hs-college-home', $contexts)) {
    $vars['logo'] = str_replace('logo.png', 'hs_logo.svg', $vars['logo']);
  }
  if (array_key_exists('ks-college', $contexts) || array_key_exists('ks-college-home', $contexts)) {
    $vars['logo'] = str_replace('logo.png', 'ks_logo.svg', $vars['logo']);
  }
  if (array_key_exists('hf-college', $contexts) || array_key_exists('hf-college-home', $contexts)) {
    $vars['logo'] = str_replace('logo.png', 'hf_logo.png', $vars['logo']);
  }
  if (array_key_exists('fh-college', $contexts) || array_key_exists('fh-college-home', $contexts)) {
    $vars['logo'] = str_replace('logo.png', 'hfh_logo.svg', $vars['logo']);
  }
  if (array_key_exists('ex-college', $contexts) || array_key_exists('ex-college-home', $contexts)) {
    $vars['logo'] = str_replace('logo.png', 'ex_logo.svg', $vars['logo']);
  }
  // replace logo with .svg
  //$vars['logo'] = str_replace(array('.jpg', '.png'), '.svg', $vars['logo']);

  //
  // get exposed search form, if available (to render it in modal dialog)
  $exposed_search_form_id = 'views_f8fbd4e2a199efa3b091b51b9cbe2130';
  $vars['exposed_search_form_modal_id'] = null;

  if (isset($vars['page']['navigation'][$exposed_search_form_id]) &&
      ($exposed_search_form = $vars['page']['navigation'][$exposed_search_form_id])) {
    $vars['exposed_search_form'] = $exposed_search_form;
    if (isset($_SESSION['hso']['exposed_search_form_modal_id'])) {
      $vars['exposed_search_form_modal_id'] = $_SESSION['hso']['exposed_search_form_modal_id'];
      $vars['exposed_search_form_modal_title'] = t('Lehrgang Suche');
    }
  }
}

/* =============================================================================
 *      Exposed search form
 * ========================================================================== */
/**
 * Adapts the exposed search form to show placeholders in search fields.
 *
 */
function pixelgarage_preprocess_views_exposed_form(&$vars) {
  //
  // add placeholders to search exposed form items
  $form = &$vars['form'];
  if ($form['#id'] != 'views-exposed-form-course-times-search-page-search-course-times') return;

  foreach ($form['#info'] as $id => $info) {
    // add the description as placeholder to the widget
    $form[$info['value']]['#attributes']['placeholder'] = $info['description'];
    $form[$info['value']]['#printed'] = false;

    // render widget again
    $field = $form[$info['value']];
    $vars['widgets'][$id]->widget = drupal_render($field);
  }

  //
  // define modal dialog trigger button
  $vars['button_text'] = t('Welcher Lehrgang, wann, wo?');
  $vars['button_search'] = t('Suchen');
  $vars['button_modal_id'] = 'modal-exposed-search-form';
  $_SESSION['hso'] = array('exposed_search_form_modal_id' => $vars['button_modal_id']);

}


/* =============================================================================
 *      Front Masonry
 * ========================================================================== */
/**
 * Adapts the proximity view, so that always two randomly chosen testimonials are visible
 * in the masonry.
 */
function pixelgarage_preprocess_views_view_proximity (&$vars) {
  $view = $vars['view'];
  if ($view->name != 'social_masonry') return;

  // add testimonials for the social masonry
  $vars['testimonials'] = _random_testimonials();
}

function _random_testimonials() {
  static $testimonials = array();
  if (empty($testimonials)) {
    // get the published testimonials
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'testimonial')
      ->propertyCondition('status', NODE_PUBLISHED);
    $result = $query->execute();
    if ($result && !empty($result['node'])) {
      $testimonials = node_load_multiple(array_keys($result['node']));
    }
  }

  // return a random testimonial list
  if (count($testimonials) <= 1) {
    $ret = array();
    foreach ($testimonials as $key => $testimonial) {
      $ret[] = array($key => node_view($testimonial, 'teaser'));
    }
    return $ret;
  }
  $keys = array_rand($testimonials, 2);
  $i1 = mt_rand(0, 9);
  return array(
    0 => array($keys[0] => node_view($testimonials[$keys[0]], 'teaser')),
    $i1 => array($keys[1] => node_view($testimonials[$keys[1]], 'teaser')),
  );
}

/* =============================================================================
 *      Course-times views
 * ========================================================================== */
/**
 * Converts the grouped tables into tabs for all course-times views.
 */
function pixelgarage_preprocess_views_view(&$vars) {
  $view = &$vars['view'];
  if ($view->name != 'course_times') return;

  //
  // split rendered rows into tables again, if any
  // create location tabs and date headers
  $vars['tabs'] = array();
  if (empty($vars['rows'])) return;

  // get view-grouping elements
  $view_groupings = explode('<div class="view-grouping">', $vars['rows']);
  array_shift($view_groupings); // delete null element at beginning
  foreach ($view_groupings as $view_grouping) {
    $content = explode('<div class="view-grouping-content">', $view_grouping);

    // get location tab text
    $tab_title = str_replace('<div class="view-grouping-header">', '', $content[0]);
    $tab_title = str_replace('</div>', '', $tab_title);
    $tabs[$tab_title] = array();

    // get date headers and tables
    $resp_tables = substr($content[1], 0, -12); // delete </div> at end
    $tables = explode('<div class="table-responsive">', $resp_tables);
    array_shift($tables); // delete null element at beginning
    $tables = array_map(function($table) { return '<div class="table-responsive">' . $table; }, $tables);
    foreach ($tables as $table) {
      $splitters = explode('caption>', $table);
      $date_title = str_replace('</', '', $splitters[1]);
      $tabs[$tab_title][$date_title] = $table;
    }
  }

  // sort arrays
  ksort($tabs);
  $vars['tabs'] = $tabs;

}


/* =============================================================================
 *      Segment page accordion
 * ========================================================================== */
/**
 * Adapts the courses accordion list in the segment page.
 */
function pixelgarage_preprocess_views_bootstrap_accordion_plugin_style(&$vars) {
  $view = &$vars['view'];
  //
  // add some additional variables for the segment courses accordion view
  $title_field = $vars['options']['title_field'];

  $vars['classes_array'][] = 'panel-group';
  $vars['label_short_desc'] = t('In Kürze');

  // Get titles with course link.
  if (isset($view->field[$title_field])) {
    foreach ($vars['view']->result as $key => $field) {
      $title = $view->style_plugin->get_field($key, $title_field);
      $vars['titles'][$key] = $title;
    }
  }

  // add course subject titles and main titles, if any
  $active_course_subject = '';
  foreach ($vars['view']->result as $key => $field) {
    $course_subject = strip_tags($view->style_plugin->get_field($key, 'field_fachgebiet'));
    $course_subject = !empty($course_subject) ? $course_subject : 'Lehrgänge';

    if ($active_course_subject != $course_subject) {
      $vars['course_subjects'][$key] = $course_subject;
      $active_course_subject = $course_subject;

      // add main titles
      if ($course_subject == 'Management und Organisation') {
        $vars['course_subject_titles'][$key] = t('Kaderschule');
        $vars['course_subject_description'][$key] = '';
      } else if ($course_subject == 'Diplomstudium HF') {
        $vars['course_subject_titles'][$key] = t('Höhere Fachschule für Wirtschaft & Informatik');
        $vars['course_subject_description'][$key] = '';
      } else if ($course_subject == 'Bachelor-Studium') {
        $vars['course_subject_titles'][$key] = t('Fachhochschule HFH');
        $vars['course_subject_description'][$key] =
          t('Mit der <a href="@hfh_url" target="_blank">Hamburger Fern-Hochschule</a> führen wir eine Exklusivität in der Schweizer Wirtschaftsbildung: An der HSO haben Sie die Möglichkeit (ohne Berufsmatura), integral im HFW-Diplomstudium exakt abgestimmte Module des Fachhochschulprogramms zu absolvieren. So schliessen Sie nach insgesamt 4½ Jahren den nach Bologna-Richtlinien anerkannten Bachelor of Arts in Betriebswirt­schaft ab. Dabei erreichen Sie nach dem 1. Jahr den Zwischenabschluss Höheres Wirtschaftsdiplom VSK/HWD, nach 3 Jahren den eidge­nössisch geschützten «Dipl. Betriebswirtschafter / -in HF» und ab dem 4. Studienjahr konzentrieren Sie sich auf die restlichen Bachelor-Module und erlangen damit den Bachelor of Arts in Betriebswirtschaft (180 ECTS).',
            array('@hfh_url' => 'http://www.hamburger-fh.de/'));
      } else {
        $vars['course_subject_titles'][$key] = '';
        $vars['course_subject_description'][$key] = '';
      }

    } else {
      $vars['course_subjects'][$key] = '';
      $vars['course_subject_titles'][$key] = '';
    }
  }

}

/* =============================================================================
 *      Contacts tab view
 * ========================================================================== */
/**
 * Adapts the contacts tab view to show only one tab per location.
 *
 */
function pixelgarage_preprocess_views_bootstrap_tab_plugin_style(&$vars) {
  $view = &$vars['view'];
  $vars['id'] = $view->vid;
  if ($view->name != 'contacts') return;

  $tab_field = $vars['options']['tab_field'];

  // create tabs for locations.
  if (isset($view->field[$tab_field])) {
    $vars['tabs'] = array();
    foreach ($vars['view']->result as $key => $contact) {
      $location = strip_tags($view->style_plugin->get_field($key, $tab_field));
      $vars['tabs'][$location][] = $key;
    }
  }
}


/* =============================================================================
 *      Banners view
 * ========================================================================== */
/**
 * Implements a fading carousel for the banners.
 */
function pixelgarage_preprocess_views_bootstrap_carousel_plugin_style(&$vars) {
  $view = &$vars['view'];
  if ($view->name != 'banners') return;

  // implements fading transition (css)
  $vars['classes_array'][] = 'carousel-fade';

  // implement interval per slide
  foreach ($view->result as $index => $result) {
    $node = $result->_field_data['nid']['entity'];

    // check if banner is valid
    $now = time();
    $start_timestamp = isset($node->field_validity_period[LANGUAGE_NONE][0]['value']) ?
      strtotime($node->field_validity_period[LANGUAGE_NONE][0]['value']) : false;
    $end_timestamp = isset($node->field_validity_period[LANGUAGE_NONE][0]['value2']) ?
      strtotime($node->field_validity_period[LANGUAGE_NONE][0]['value2']) : $now + 1;
    if ($start_timestamp && $end_timestamp && ($start_timestamp > $now || $end_timestamp < $now)) {
      // banner is not valid
      continue;
    }

    // set banner
    $vars['banners'][] = $vars['rows'][$index];

    // set slide duration if any
    $interval = isset($node->field_interval[LANGUAGE_NONE][0]['value']) ?
      $node->field_interval[LANGUAGE_NONE][0]['value']*1000 : 10000;
    $vars['intervals'][] = $interval;
  }
}


/* =============================================================================
 *      Remote forms
 * ========================================================================== */
/**
 * Preprocess panel pane for remote forms to add the GUID (found in the URL) to the iFrame src attribute.
 *
 * @param $vars
 */
function pixelgarage_preprocess_panels_pane(&$vars) {
  $pane = $vars['pane'];

  // process only RemoteForm pane
  if (isset($pane->configuration['admin_title']) && $pane->configuration['admin_title'] == "RemoteForm") {
    if (array_key_exists('id', $_GET)) {
      // add the GUID to the iFrame src attribute, if any
      $res = explode('src="', $vars['content']);
      $content = $res[0] . 'src="';
      $pos = strpos($res[1], '" ');
      $guid = "&id=" . $_GET['id'];
      $content .= substr_replace($res[1], $guid, $pos, 0);

      $vars['content'] = $content;
    }
  }

}



