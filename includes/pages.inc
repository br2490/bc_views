<?php

// THIS IS NOT YET A FEATURE! THIS IS A PLACEHOLDER FILE.

// This doesn't belong here, but this isn't yet integrated so it can stay...
function bc_views_menu() {
	$page['studentpublications'] = array(
		'title' => 'Student Publications',
		'page callback' => 'bc_views_studentPublications',
		'access callback' => TRUE,
		'type' => MENU_CALLBACK, 
		);
	return $page;
}

function bc_views_studentPublications() {
  return array('#markup' => '<p>' . t('I\'m not being used yet!') . '</p>');
}

?>