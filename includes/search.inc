<?php
/**
 * @file
 * Description TK.
 */


/**
 * Implements hook_form_FORM_ID_alter()
 * We're gonna use our own submit handler for our content type.
 */
function bc_views_form_islandora_solr_advanced_search_form_alter(&$form, &$form_state) {
	global $_bc_views_solr_view;

	if ($_bc_views_solr_view !== null || isset($form_state['build_info']['args']['barnard_custom_view'])) {
		unset($form['#submit']);
		$form['#submit'][] = 'bc_views_advanced_search_form_submit';
		$form_state['build_info']['args']['barnard_custom_view'] = TRUE;
	}

}

/**
 * Islandora Solr advanced search form submit callback.
 *
 * @param array $form
 *   An associative array containing form structure.
 * @param array $form_state
 *   An associative array containing form state.
 *
 * @see islandora_solr_advanced_search_form()
 */
function bc_views_advanced_search_form_submit($form, &$form_state) {
  module_load_include('inc', 'islandora_solr', 'includes/utilities');
  global $_islandora_solr_queryclass;
  // Collect query values.
  $query_array = array();
  foreach ($form_state['values']['terms'] as $term) {
    $field = $term['field'];
    $search = trim($term['search']);
    $search = islandora_solr_replace_slashes($search);
    $boolean = (isset($term['boolean'])) ? $term['boolean'] : variable_get('islandora_solr_search_boolean', 'user');
    $field = islandora_solr_lesser_escape($field);
    // Add query.
    if (!empty($search)) {
      $query_array[] = array(
        'search' => "$field:($search)",
        'boolean' => $boolean,
      );
    }
  }
  // Create query.
  $query = '';
  $i = 0;
  foreach ($query_array as $term) {
    $query .= $term['search'];
    if (count($query_array) - 1 != $i) {
      $query .= ' ' . $term['boolean'] . ' ';
    }
    $i++;
  }
  // Check if query is empty.
  if (empty($query)) {
    $query = '*:*';
  }

  // (Barnard custom code starts here.)
  // For our content type we always respect filters, so we avoid unessecary logic.

  // // Handle filters.
  // $filter = '';
  // if (isset($form_state['values']['islandora_solr_allow_preserve_filters']) && $form_state['values']['islandora_solr_allow_preserve_filters']) {
  // $filter = isset($_GET['f']) ? $_GET['f'] : '';
  // }

  // Get our current params.
 	$params = isset($_islandora_solr_queryclass->internalSolrParams) ? $_islandora_solr_queryclass->internalSolrParams : array();

  // Unset our collection query from the params. We're building a new CQ below.
  if (isset($params['cq'])) {
    unset($params['cq']);
  }

 	// Build our search string with params and redirect back to collection page.
 	$search_string = array_merge(array('cq' => $query), $params);
	$form_state['redirect'] = array(
    current_path(),
    array(
      'query' => $search_string,
    ),
  );
 
  // Navigate to results page.
  // $form_state['redirect'] = array(
  //   ISLANDORA_SOLR_SEARCH_PATH . "/$query",
  //   $filter ? array('query' => array('f' => $filter)) : array(),
  // );
}



## REGION TEMPORARY SEARCH TESTING
/*
// This is a temporary search block in an attempt to get SOLR working with our current setup.
function bc_views_search_form($form, &$form_state) {
  $form['bc_views_search'] = array(
  	'#type' => 'container',
    '#attributes' => array(
      	'class' => array(
        'container-inline',
      ),
    ),
  );
  $form['bc_views_search']["query"] = array(
    '#size' => '15',
    '#type' => 'textfield',
    '#title' => t('Search: '),
    // @todo Should this be the searched value?
    '#default_value' => '',
  );
  $form['bc_views_search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Go!'),
  );

  return $form;
}

function bc_views_search_form_submit($form, &$form_state) {
	global $_islandora_solr_queryclass; 
	global $_bc_views_solr_view;

	$search_string = islandora_solr_replace_slashes($form_state['values']['query']);

	$params = isset($_islandora_solr_queryclass->internalSolrParams) ? $_islandora_solr_queryclass->internalSolrParams : array();

	$query = array_merge($params, array('cq' => $search_string));

  $form_state['redirect'] = array(
    current_path(), 
    array(
      'query' => $query,
    ),
  );
}
*/
## END REGION TEMPORARY SEARCH TESTING

?>