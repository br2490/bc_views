<?php 
/**
 * @file
 * Collection page CONTENT modifications.
 */

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
	// we've already render and add it to our pages ['content'].
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

?>