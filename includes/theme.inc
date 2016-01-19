<?php
/**
 * @file
 * Collection page THEME modifications.
 */

/**
 * Implements theme_preprocess_page().
 */
function bc_views_preprocess_page(&$vars) {
	// This code allows page level themeing based on the type of NODE.
	if (isset($vars['node']) && $vars['node']->type == 'bc_views_solr_view') {
		if (!(bool)strpos(current_path(),'edit')) {
			$vars['theme_hook_suggestions'][] = 'page__node__' . $vars['node']->type;
		} // else { do some edit page theming or nothing... }
	}
}

/**
 * Implements hook_theme().
 */
function bc_views_theme($existing, $type, $theme, $path) {
  $theme = array();
  $theme['page__node__bc_views_solr_view'] = array(
    'render element' => 'content',
    'base hook' => 'page',
    'template' => 'page--node--bc_views_solr_view',
    'path' => drupal_get_path('module', 'bc_views') . '/templates',
   );
  return $theme;
}


?>