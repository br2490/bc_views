<?php
## TODO DESCRIBE FILE

/**
 * Implements hook_islandora_solr_query().
 */
function bc_views_islandora_solr_query($islandora_solr_query) {
	global $_bc_views_solr_view;
	if (!$_bc_views_solr_view['view']) {
		return;
	}

	/* Has the user entered a search term? Here is where we modify our Collection View...
	 * Formost we need to find if any string were used in the SOLR FQ, and change them to tokenized fields (i.e. 'ms' becomes 'mt')
	 * Then we set our Solr Query to the user's term with a CATCH_ALL so we get as much for our search as possible.
	 * Finally, we return the ISQP and stop the function from continuing.
	 */
	if (isset(drupal_get_query_parameters()['cq'])) {

		// Tokenize and set field query limiter
		$content_query = str_replace('_ms:', '_mt:', $_bc_views_solr_view['content_query']);
		$islandora_solr_query->solrParams['fq'][] = $content_query;

		// Do our thang...
		return $islandora_solr_query;
	} 

	// Set field query limiter.
	$islandora_solr_query->solrParams['fq'][] = $_bc_views_solr_view['content_query'];
	
	return $islandora_solr_query;
}

/**
 * Implements hook_islandora_solr_query_alter().
 * Modifies query before execution....
 */
function bc_views_islandora_solr_query_alter(&$islandora_solr_query) {
	global $_bc_views_solr_view;

	// If it is not one of our UNIQUE queries, return;
	if (!$_bc_views_solr_view['view']) {
		return;
	}

	// I need to figure out pagination and why ROWS isn't restricting the results from SOLR. Fucked up...
	// This leads to pagination, not RESULT limits.
	$islandora_solr_query->solrLimit = $_bc_views_solr_view['rows'];

	// Update facets to only those selected...
	$islandora_solr_query->solrParams['facet.field'] = $_bc_views_solr_view['facets'];
}

// !!
// At this point the SOLR Query is executed!!!
// !!

/**
 * Implements hook_islandora_solr_query_result().
 */
// function bc_views_islandora_solr_query_result($result) {
// }

/**
 * Implements hook_islandora_solr_results_alter().
 */
// function bc_views_islandora_solr_results_alter(&$search_results, $query_processor) {
// }

/**
 * Implements hook_form_FORM_ID_alter()
 * We're gonna use our own submit handler for our content type.
 */
function bc_views_form_islandora_solr_advanced_search_form_alter(&$form, &$form_state) {
	global $_bc_views_solr_view;

	if (isset($_bc_views_solr_view) || isset($form_state['build_info']['args']['barnard_custom_view'])) {
		unset($form['#submit']);
		$form['#submit'][] = 'bc_views_advanced_search_form_submit';
		$form_state['build_info']['args']['barnard_custom_view'] = TRUE;
	}

}

## MODIFIED SUBMIT FUNCTION (this is such a sexy af function <3)
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

  ## The custom code...
  // For our content type we always respect filters, so we're just avoiding unessecary logic.

  // // Handle filters.
  // $filter = '';
  // if (isset($form_state['values']['islandora_solr_allow_preserve_filters']) && $form_state['values']['islandora_solr_allow_preserve_filters']) {
  //   $filter = isset($_GET['f']) ? $_GET['f'] : '';
  // }

  // Get our current params...
 	$params = isset($_islandora_solr_queryclass->internalSolrParams) ? $_islandora_solr_queryclass->internalSolrParams : array();

 	// Build our search string w/ params.
 	$search_string = array_merge($params, array('cq' => $query));

 	// Pump it up, pump up the jams.
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
  ## End of the custom code.
}
## END MODIFIED SUBMIT FUNCTION

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