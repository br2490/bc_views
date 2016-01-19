<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728096
 */

/**
 * Override or insert variables into the maintenance page template.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("maintenance_page" in this case.)
 */
/* -- Delete this line if you want to use this function
function barnard_theme_preprocess_maintenance_page(&$variables, $hook) {
  // When a variable is manipulated or added in preprocess_html or
  // preprocess_page, that same work is probably needed for the maintenance page
  // as well, so we can just re-use those functions to do that work here.
  barnard_theme_preprocess_html($variables, $hook);
  barnard_theme_preprocess_page($variables, $hook);
}
// */

/**
 * Override or insert variables into the html templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("html" in this case.)
 */
/* -- Delete this line if you want to use this function
function barnard_theme_preprocess_html(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');

  // The body tag's classes are controlled by the $classes_array variable. To
  // remove a class from $classes_array, use array_diff().
  //$variables['classes_array'] = array_diff($variables['classes_array'], array('class-to-remove'));
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
/* -- Delete this line if you want to use this function
function barnard_theme_preprocess_page(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the node templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
/* -- Delete this line if you want to use this function
function barnard_theme_preprocess_node(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');

  // Optionally, run node-type-specific preprocess functions, like
  // barnard_theme_preprocess_node_page() or barnard_theme_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $variables['node']->type;
  if (function_exists($function)) {
    $function($variables, $hook);
  }
}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function barnard_theme_preprocess_comment(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the region templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("region" in this case.)
 */
/* -- Delete this line if you want to use this function
function barnard_theme_preprocess_region(&$variables, $hook) {
  // Don't use Zen's region--sidebar.tpl.php template for sidebars.
  //if (strpos($variables['region'], 'sidebar_') === 0) {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('region__sidebar'));
  //}
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function barnard_theme_preprocess_block(&$variables, $hook) {
  // Add a count to all the blocks in the region.
  // $variables['classes_array'][] = 'count-' . $variables['block_id'];

  // By default, Zen will use the block--no-wrapper.tpl.php for the main
  // content. This optional bit of code undoes that:
  //if ($variables['block_html_id'] == 'block-system-main') {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('block__no_wrapper'));
  //}
}
// */

/**
 * Implements hook_preprocess_page().
 */
function barnard_theme_preprocess_page(&$vars) {
  // This code allows page level themeing based on NODE TYPE. Coolio.
  if (isset($vars['node'])) {
    $vars['theme_hook_suggestions'][] = 'page__node__' . $vars['node']->type;
  }

  // If we have bc_islandora and this is the front page, invoke
  // _bc_islandora_featured() and set  $vars['page']['footer']['front_caption'].
  if (module_exists('bc_islandora') && $vars['is_front']) {
    module_load_include('inc', 'bc_islandora', 'includes/bc_islandora.theme');
    $vars['page']['footer']['front_caption'] = array(
      '#markup' => _bc_islandora_featured(),
      '#prefix' => '<div id="block-views-featured-block">',
      '#suffix' => '</div>',
    );
  }
  // If we have bc_islandora, this is NOT the front page, and this is not a
  // search result page, call bc_islandora's custom breadcrumb theming method
  // and set $vars['bc_breadcrumb'].
  if (module_exists('bc_islandora') && !$vars['is_front'] && arg(1) != 'search') {
    $vars['bc_breadcrumb'] = theme('bc_islandora_breadcrumb', array('breadcrumb' => array()));
  }
  // If we have service_links, set $vars['socialmedia'].
  if (module_exists('service_links') && _service_links_match_path()) {
    $vars['socialmedia'] = implode('', service_links_render(NULL));
  }
  // If we have bc_islandora and this is an exhibit node, add our exhibit js
  // and css.
  if (module_exists('bc_islandora') && ((isset($vars['node']) && $vars['node']->type == 'exhibition') || (count(arg()) == 1 && arg(0) == 'exhibits'))) {
    drupal_add_js(drupal_get_path('module', 'bc_islandora') . '/js/dc_exhibit.js');
    drupal_add_css(drupal_get_path('module', 'bc_islandora') . '/css/dc_exhibit.css');
  }
  if (arg(0) == 'islandora' && arg(1) == 'object') {
    drupal_add_js(array('permalink_path' => $_GET['q']), 'setting');
    drupal_add_js(drupal_get_path('theme', 'barnard_theme') . '/js/permalink.js');
  }
  if (arg(0) == 'islandora' && arg(1) == 'search') {
    drupal_add_js(drupal_get_path('theme', 'barnard_theme') . '/js/hide_datepicker.js');
  }

}

/**
 * Implements hook_preprocess_node().
 */
function barnard_theme_preprocess_node(&$vars) {
  $node = $vars['node'];
  // If we have bc_islandora and this is an exhibit node, invoke bc_islandora's
  // exhibit theming method and set $vars['exhibtion'].
  if (module_exists('bc_islandora') && $node->type == 'exhibition') {
    module_load_include('inc', 'bc_islandora', 'includes/bc_islandora.theme');
    $vars['exhibition'] = theme('bc_islandora_exhibition', array('node' => $node));
  }
}

/**
 * Implements hook_preprocess_islandora_basic_collection_wrapper().
 */
function barnard_theme_preprocess_islandora_basic_collection_wrapper(&$vars) {
  $object = $vars['islandora_object'];
  if (isset($object['MODS']) && $mods = simplexml_load_string($object['MODS']->getContent(NULL))) {
    $identifier = (string) $mods->identifier;
    $id_prefix = preg_replace('/^BC/', '', array_shift(explode('-', array_shift(explode('_', $identifier)))));

    // If this is a BC12 object, $vars['student_pubs'] is TRUE.
    if ($object->id == variable_get('bc_islandora_student_pubs_pid', 'islandora:1022') || $id_prefix == '12') {
      $vars['student_pubs'] = TRUE;
    }
  }
}

/**
 * Implements hook_preprocess_islandora_book_book().
 */
function barnard_theme_preprocess_islandora_book_book(&$vars) {
  $object = $vars['object'];
  if (module_exists('bc_islandora')) {
    module_load_include('inc', 'bc_islandora', 'includes/bc_islandora.theme');
    // Provide a link to this object's PDF datastream via $vars['dl_links'].
    $vars['dl_links'] = _bc_islandora_dl_links($object, array('PDF'));
    // If this object's parent collection's pid is the same as our database
    // variable bc_islandora_documents_pid, the answer is YES.
    if (_bc_islandora_is_document($object)) {
      drupal_add_js(libraries_get_path('openseadragon') . '/openseadragon.js');
      $vars['viewer'] = theme('bc_islandora_newspaper_issue', array('object' => $object));
    }
  }
}

/**
 * Implements hook_preprocess_islandora_book_page().
 */
function barnard_theme_preprocess_islandora_book_page(&$vars) {
  $object = $vars['object'];
  if (module_exists('bc_islandora')) {
    module_load_include('inc', 'bc_islandora', 'includes/bc_islandora.theme');
    // Provide a link to this object's JPG datastream via $vars['dl_links'].
    $vars['dl_links'] = _bc_islandora_dl_links($object, array('JPG'));
  }
}

/**
 * Implements hook_preprocess_islandora_large_image().
 */
function barnard_theme_preprocess_islandora_large_image(&$vars) {
  if (module_exists('bc_islandora')) {
    module_load_include('inc', 'bc_islandora', 'includes/bc_islandora.theme');
    // Provide a link to this object's JPG datastream via $vars['dl_links'].
    $vars['dl_links'] = _bc_islandora_dl_links($vars['islandora_object'], array('JPG'));
  }
}

/**
 * Implements hook_CMODEL_PID_islandora_solr_object_result_alter().
 */
function barnard_theme_islandora_newspaperpagecmodel_islandora_solr_object_result_alter(&$search_results, $query_processor) {
  $query = trim($query_processor->solrQuery);
  if (empty($query)) {
    unset($search_results['object_url_params']['solr']);
  }
  else {
    $search_results['object_url_params']['solr']['params'] = array('defType' => 'dismax');
  }
}

/**
 * Implements hook_CMODEL_PID_islandora_solr_object_result_alter().
 *
 * Add page viewing fragment and search term to show all search results within
 * book on page load.
 */
function barnard_theme_islandora_bookCModel_islandora_solr_object_result_alter(&$search_results, $query_processor) {
  $view_types = array(
    "1" => "1up",
    "2" => "2up",
    "3" => "thumb",
  );

  $field_match = array(
    'catch_all_fields_mt',
    'OCR_t',
    'text_nodes_HOCR_hlt',
  );

  $field_term = '';
  $fields = preg_split('/OR|AND|NOT/', $query_processor->solrQuery);
  foreach ($fields as $field) {
    if (preg_match('/^(.*):\((.*)\)/', $field, $matches)) {
      if (isset($matches[1]) && in_array($matches[1], $field_match)) {
        $field_term = ((isset($matches[2]) && $matches[2]) ? $matches[2] : '');
        break;
      }
    }
  }

  if ($field_term) {
    $search_term = trim($field_term);
  }
  elseif ($query_processor->solrDefType == 'dismax' || $query_processor->solrDefType == 'edismax') {
    $search_term = trim($query_processor->solrQuery);
  }

  $ia_view = variable_get('islandora_internet_archive_bookreader_default_page_view', "1");
  $search_results['object_url_fragment'] = "page/1/mode/{$view_types[$ia_view]}";
  if (!empty($search_term)) {
    $search_results['object_url_fragment'] .= "/search/" . rawurlencode($search_term);
  }
}

/**
 * Implements hook_CMODEL_PID_islandora_solr_object_result_alter().
 *
 * Replaces the url for the search result to be the book's url, not the page.
 * The page is added as a fragment at the end of the book url.
 */
function barnard_theme_islandora_pageCModel_islandora_solr_object_result_alter(&$search_results, $query_processor) {
  // Grab the names of the appropriate solr fields from the db.
  $parent_book_field_name = variable_get('islandora_book_parent_book_solr_field', 'RELS_EXT_isMemberOf_uri_ms');
  $page_number_field_name = variable_get('islandora_paged_content_page_number_solr_field', 'RELS_EXT_isSequenceNumber_literal_ms');
  // If:
  // there's an object url AND
  // there's a solr doc AND
  // the solr doc contains the parent book AND
  // the solr doc contains the page number...
  if (isset($search_results['object_url']) &&
    isset($search_results['solr_doc']) &&
    isset($search_results['solr_doc'][$parent_book_field_name]) &&
    count($search_results['solr_doc'][$parent_book_field_name]) &&
    isset($search_results['solr_doc'][$page_number_field_name]) &&
    count($search_results['solr_doc'][$page_number_field_name])) {
    // Replace the result url with that of the parent book and add the page
    // number as a fragment.
    $book_pid = preg_replace('/info\:fedora\//', '', $search_results['solr_doc'][$parent_book_field_name][0], 1);
    $page_number = $search_results['solr_doc'][$page_number_field_name][0];

    if (islandora_object_access(ISLANDORA_VIEW_OBJECTS, islandora_object_load($book_pid))) {
      $search_results['object_url'] = "islandora/object/$book_pid";
      $view_types = array(
        "1" => "1up",
        "2" => "2up",
        "3" => "thumb",
      );
      $ia_view = variable_get('islandora_internet_archive_bookreader_default_page_view', "1");
      $search_results['object_url_fragment'] = "page/$page_number/mode/{$view_types[$ia_view]}";

      $field_match = array(
        'catch_all_fields_mt',
        'OCR_t',
        'text_nodes_HOCR_hlt',
      );

      $field_term = '';
      $fields = preg_split('/OR|AND|NOT/', $query_processor->solrQuery);
      foreach ($fields as $field) {
        if (preg_match('/^(.*):\((.*)\)/', $field, $matches)) {
          if (isset($matches[1]) && in_array($matches[1], $field_match)) {
            $field_term = ((isset($matches[2]) && $matches[2]) ? $matches[2] : '');
            break;
          }
        }
      }

      if ($field_term) {
        $search_term = trim($field_term);
      }
      elseif ($query_processor->solrDefType == 'dismax' || $query_processor->solrDefType == 'edismax') {
        $search_term = trim($query_processor->solrQuery);
      }

      if (!empty($search_term)) {
        $search_results['object_url_fragment'] .= "/search/" . rawurlencode($search_term);
      }
    }
  }
}
