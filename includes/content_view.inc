<?php 
/**
 * @file
 * Collection content page display and config.
 */

/**
 * Implements hook_node_view()
 * This is where we display our content type to the end-user.
 */
function bc_views_node_view($node, $view_mode = 'full') {
	if ($node->type == 'bc_views_solr_view') {

		// Get our globals.
		global $_bc_views_solr_view;
		global $_islandora_solr_queryclass;

		// We're on a bc_views_solr_view page...
		$_bc_views_solr_view['view'] = TRUE;

		// These are our content variables that the end-user has little interaction with. 
		// These DEFINE what is present on the content page, and also help us with search.
		// Everything gets saved in the content's global for reasons ('reasons...')
		
		// Configure the solr facets that will be displayed on the final content page.
		foreach (field_get_items('node', $node, 'bc_views_query_facets') as $field) {
			$_bc_views_solr_view['facets'][] = $field['value'];
		}

		// Define and save our starting solr query, and results per page.
		$bc_views_solr_query = field_get_items('node', $node, 'bc_views_solr_query');
		$bc_views_query_rows = field_get_items('node', $node, 'bc_views_query_rows');
		
		$_bc_views_solr_view['content_query'] = $bc_views_solr_query[0]['value'];
		$_bc_views_solr_view['rows'] = $bc_views_query_rows[0]['value'];

		// For our content type, we want to default to the grid view. If the user selects 
		// something else we do not want to override them back to the grid view.
		!isset($_GET['display']) ? $_GET['display'] = 'grid' : NULL;

		// The important stuff:
		// Foremost we rely on islandora_solr() to return a search result - that will be our primary content.
		// This reliance on islandora_solr let us do amazing things fairly simply.
		
		// We use a param called "cq" if the user has preformed a search from our collection page.
		// If it's not there we display the content as it was configured by the curator.
		if (!in_array('cq', array_keys(drupal_get_query_parameters()))) {
			$solr_result_output = islandora_solr(); // See hook_islandora_solr()
		}
		else {
			$query_params = drupal_get_query_parameters();
			$collection_query = $query_params['cq'];
			$solr_result_output = islandora_solr($collection_query); // See hook_islandora_solr_query() below.
			// @TODO: Add a catch for results == 0 to return to default collection view, with a prompt.
		}


		// Build the rest of our block's content - include the switch (list/grid) and sort blocks.
		$node->content['sort_order_block'] = array(
			'#markup' => block_content_render('islandora_solr', 'sort'),
			'#weight' => '2',
			);

		$node->content['switch_block'] = array(
			'#markup' => block_content_render('islandora_solr', 'display_switch'),
			'#weight' => '2',
			);

		$node->content['results'] = array(
			'#markup' => $solr_result_output,
			'#weight' => '3',
		);

	}
}


/*
 * Implements hook_block_list_alter().
 */
function bc_views_block_list_alter(&$blocks) {
	global $_bc_views_solr_view;
	global $theme_key;

	// If it is not one of our unique queries, return;
	if (!$_bc_views_solr_view['view']) {
		return;
	}

	// Find the display_switch block that, for some unknown reason, is 
	// displayed on all pages on our site (or by default)
	// We do not want *this* display_switch block, since 
	// we already render and add it to our pages ['content'].
	foreach($blocks as $block) {
		if($block->delta == 'display_switch') {
			unset($blocks[$block->bid]);
		}
	}

	// We're going to add our sidebar content, mainly the facet block and
  // advanced search block. Both of these are from the islandora_solr module.

	$additional_blocks = array(
    'islandora_solr' => array(
			'advanced' => array(
        'title' => 'Search this collection',
        'region' => 'sidebar_first',
        'weight' => 0,
      ),
      'basic_facets' => array(
        'title' => 'Filter this collection',
        'region' => 'sidebar_first',
        'weight' => 1,
      ),
		),
	);

	// Fetch the additional blocks, setting the theme, region and weight.	
	foreach ($additional_blocks as $module_name => $addl_block) {
		foreach ($addl_block as $bid => $block_info) {
      $block = block_load($module_name, $bid);
      $block->theme = $theme_key;
      $block->title = $block_info['title'];
      $block->region = $block_info['region'];
      $block->weight = $block_info['weight'];
      $blocks[] = $block;
		}
	}

  // Sort the blocks
  usort($blocks, "custom_sort");
}

/**
 * Sorting function for blocks as objects. Why drupal only has sort functions for arrays is beyond me...
 */
function custom_sort($a,$b) {
  return $a->weight>$b->weight;
}

/**
 * Generates a rendered block from $module with name $function.
 */
function block_content_render($module, $function) {
	$block = block_load($module, $function);
	$block_renderable = _block_get_renderable_array(_block_render_blocks(array($block)));
	$render = drupal_render($block_renderable);
	return $render;	
}


## REGION: Facet block modifications.
/**
 * Implements hook_form_BASE_ID_alter()
 * For our content type we want to simplify the facet block by hiding a few elements.
 */
function bc_views_form_islandora_solr_range_slider_form_alter(&$form, &$form_state) {
	global $_bc_views_solr_view;

	// If it is not one of our unique queries, return;
	if (!$_bc_views_solr_view['view']) {
		return;
	}

	// We need to access the theme layer to hide the display of the title "Date Created"
	// @see: https://github.com/Islandora/islandora_solr_search/blob/7.x/theme/theme.inc#L108

	// Get rid of datepicker bar and the corresponding submit.
 	$form['markup']['#access'] = FALSE;
	$form['range_slider_submit']['#access'] = FALSE;
}

/**
 * Implements hook_form_BASE_ID_alter()
 * For our content type we want to simplify the facet block by hiding a few elements.
 */
function bc_views_form_islandora_solr_date_filter_form_alter(&$form, &$form_state) {
	global $_bc_views_solr_view;

	// If it is not one of our unique queries, return;
	if (!$_bc_views_solr_view['view']) {
		return;
	}

	// Modify the markup to class 'expanded' and remove the hide/show link:
	$form['date_range_expand']['#markup'] = '<span class="toggle-date-range-filter date-range-expanded"></span>';
}
## END REGION


## REGION HOOK ISLANDORA SEARCH
// THIS area is a KEY to our content display, and not searching within the collection.
// THEREFORE it is in content_view.inc and not search.inc.

/**
 * Implements hook_islandora_solr_query().
 */
function bc_views_islandora_solr_query($islandora_solr_query) {
	global $_bc_views_solr_view;
	if (!$_bc_views_solr_view['view']) {
		return;
	}

	// Has the user entered a search term? Here is where we modify our collection
  // view... First we need to find if any strings were used in the solr fq, and
  // change them to tokenized fields (i.e. 'ms' becomes 'mt'). Then we set our
  // solr query to the user's term with a catch-all so we get as much for our
  // search as possible. Finally, we return the ISQP and stop the function from
  // continuing.
	if (in_array('cq', array_keys(drupal_get_query_parameters()))) {

		// Tokenize and set field query limit.
		$content_query = str_replace('_ms:', '_mt:', $_bc_views_solr_view['content_query']);
		$islandora_solr_query->solrParams['fq'][] = $content_query;

		// Return our search ISQP.
		return $islandora_solr_query;
	} 

	// Set field query limit.
	$islandora_solr_query->solrParams['fq'][] = $_bc_views_solr_view['content_query'];

	return $islandora_solr_query;
}

/**
 * Implements hook_islandora_solr_query_alter().
 * Modifies query before execution.
 */
function bc_views_islandora_solr_query_alter(&$islandora_solr_query) {
	global $_bc_views_solr_view;

	// If it is not one of our unique queries, return.
	if (!$_bc_views_solr_view['view']) {
		return;
	}

	// I need to figure out pagination and why 'rows' isn't restricting the
  // results from solr. This leads to pagination, not result limits.
	$islandora_solr_query->solrLimit = $_bc_views_solr_view['rows'];

	// Update facets to only those selected...
	$islandora_solr_query->solrParams['facet.field'] = $_bc_views_solr_view['facets'];
}

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


## END REGION

?>