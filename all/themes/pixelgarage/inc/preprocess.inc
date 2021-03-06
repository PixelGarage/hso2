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

  //
  // add facebook pixel to all pages
  $fb_pixel = <<<FB_PXL
<!-- Facebook Pixel Code -->
<script>
!function(f,b,e,v,n,t,s)
{if(f.fbq)return;n=f.fbq=function(){n.callMethod?n.callMethod.apply(n,arguments):n.queue.push(arguments)};
if(!f._fbq)f._fbq=n;n.push=n;n.loaded=!0;n.version='2.0';
n.queue=[];t=b.createElement(e);t.async=!0;
t.src=v;s=b.getElementsByTagName(e)[0];
s.parentNode.insertBefore(t,s)}(window,document,'script','https://connect.facebook.net/en_US/fbevents.js');
fbq('init', '129852244291096'); 
fbq('track', 'PageView');
</script>
<noscript>
 <img height="1" width="1" src="https://www.facebook.com/tr?id=129852244291096&ev=PageView&noscript=1"/>
</noscript>
<!-- End Facebook Pixel Code -->
FB_PXL;
  $element = array(
    '#type' => 'markup',
    '#markup' => $fb_pixel,
    '#weight' => 1000,
  );
  drupal_add_html_head($element, 'facebook_pixel');

  //
  // add HOTJAR tracking code
  $hotjar = <<<HOTJAR
<!-- Hotjar Tracking Code for www.hso.ch -->
  (function(h,o,t,j,a,r){
      h.hj=h.hj||function(){(h.hj.q=h.hj.q||[]).push(arguments)};
      h._hjSettings={hjid:749917,hjsv:6};
      a=o.getElementsByTagName('head')[0];
      r=o.createElement('script');r.async=1;
      r.src=t+h._hjSettings.hjid+j+h._hjSettings.hjsv;
      a.appendChild(r);
  })(window,document,'https://static.hotjar.com/c/hotjar-','.js?sv=');
HOTJAR;
  drupal_add_js($hotjar, 'inline');

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
}

/* =============================================================================
 *      Course node adaptation
 * ========================================================================== */
function pixelgarage_ds_pre_render_alter(&$layout_render_array, $context, &$vars) {
  // check node type
  if ($context['entity_type'] == 'node' && $context['bundle'] == 'course') {
    //
    // add course tabs to bottom region in course
    $layout_render_array['bottom'][] = $layout_render_array['left'][1];
    unset($layout_render_array['left'][1]);
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

  // Get titles with course link.
  if (isset($view->field[$title_field])) {
    $i = 0;
    foreach ($view->result as $key => $row) {
      $title = $view->style_plugin->get_field($key, $title_field);
      $category = strip_tags($view->style_plugin->get_field($key, 'field_fachgebiet'));
      $category = !empty($category) ? $category : t('Lehrgänge');
      $subcategory = strip_tags($view->style_plugin->get_field($key, 'field_course_subcategory'));
      $subcategory = !empty($subcategory) ? $subcategory : '*';
      $weight = $view->style_plugin->get_field($key, 'field_weight');
      $weight = !empty($weight) ? $weight . '_' . $i++ : 'w' . $i++;
      $vars['titles'][$key] = $title;
      $vars['course_categories'][$category][$subcategory][$weight] = $key;
      if (stripos($category, 'fachschule') !== false) {
        $vars['title_classes'][$key] = 'course-category-hf';
      }
      else if (stripos($category, 'fachhochschule') !== false) {
        $vars['title_classes'][$key] = 'course-category-fh';
      }
      else if (stripos($category, 'executive') !== false) {
        $vars['title_classes'][$key] = 'course-category-ex';
      }
      else {
        $vars['title_classes'][$key] = strtolower(drupal_clean_css_identifier($category));
      }
    }

    // sort arrays
    //ksort($vars['course_categories']);
    foreach ($vars['course_categories'] as $category => $subcategories) {
      ksort($subcategories);
      $vars['course_categories'][$category] = $subcategories;
      foreach ($subcategories as $subcategory => $weights) {
        ksort($weights);
        $vars['course_categories'][$category][$subcategory] = $weights;
      }
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
  if ($view->name == 'contacts') {
    //
    // create tabs for contacts
    $tab_field = $vars['options']['tab_field'];

    if (isset($view->field[$tab_field])) {
      $vars['tabs'] = array();
      foreach ($vars['view']->result as $key => $contact) {
        $location = strip_tags($view->style_plugin->get_field($key, $tab_field));
        $vars['tabs'][$location][] = $key;
      }
    }
  }
  else if ($view->name == 'locations') {
    //
    // create map infos for location tabs
    $vars['locations'] = array();
    foreach ($view->result as $key => $location) {
      // get fields of each location
      $address = $location->field_field_address[0]['raw'];
      $vars['locations'][$key]['organisation_name'] = check_plain($address['organisation_name']);
      $vars['locations'][$key]['thoroughfare'] = check_plain($address['thoroughfare']);
      $vars['locations'][$key]['country'] = check_plain($address['country']);
      $vars['locations'][$key]['postal_code'] = check_plain($address['postal_code']);
      $vars['locations'][$key]['locality'] = check_plain($address['locality']);
      $vars['locations'][$key]['phone_number'] = check_plain($address['phone_number']);
      $vars['locations'][$key]['email'] = $location->field_field_email[0]['rendered']['#markup'];
      $vars['locations'][$key]['picture'] = $view->style_plugin->get_field($key, 'field_picture');
      // create map location
      $vars['locations'][$key]['map_location'] = urlencode($address['thoroughfare'] . ', ' . $address['postal_code'] . ' ' . $address['locality']);
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
    if ($start_timestamp && $end_timestamp && ($start_timestamp <> $end_timestamp) && ($start_timestamp > $now || $end_timestamp < $now)) {
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



