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

/**
 * Adapts the courses accordion list in the segment page.
 */
function pixelgarage_preprocess_views_bootstrap_accordion_plugin_style(&$vars) {
  $view = &$vars['view'];
  if ($view->name != 'courses' && $view->current_display != 'block_segment_courses') return;

  //
  // add some additional variables for the segment courses accordion view
  $title_field = $vars['options']['title_field'];

  $vars['classes_array'][] = 'panel-group';
  $vars['label_short_desc'] = t('In Kürze');

  // Get titles and detail links.
  if (isset($view->field[$title_field])) {
    foreach ($vars['view']->result as $key => $field) {
      $title = $view->style_plugin->get_field($key, $title_field);
      $vars['titles'][$key] = strip_tags($title);

      // create detail links
      if ($pos = strpos($title, '>')) {
        $vars['details'][$key] = substr($title, 0, $pos+1) . t('Details') . '</a>';
      }
    }
  }

  // get course subjects
  $active_course_subject = '';
  foreach ($vars['view']->result as $key => $field) {
    $course_subject = strip_tags($view->style_plugin->get_field($key, 'field_fachgebiet'));

    if ($active_course_subject != $course_subject) {
      $vars['course_subjects'][$key] = $course_subject;
      $active_course_subject = $course_subject;

      // add main titles
      if ($course_subject == 'Management und Organisation') {
        $vars['course_subject_titles'][$key] = 'Berufsakademie';
      } else if ($course_subject == 'Diplomstudium HF') {
        $vars['course_subject_titles'][$key] = 'Höhere Fachschule für Wirtschaft';
      } else {
        $vars['course_subject_titles'][$key] = '';
      }

    } else {
      $vars['course_subjects'][$key] = '';
      $vars['course_subject_titles'][$key] = '';
    }
  }

}

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



