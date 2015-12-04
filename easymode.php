<?php
//Default to the GRID display, else let the user decide.
if (!$_GET['display']) {
    $_GET['display'] = 'grid';
}
// Query string
$query = 'RELS_EXT_isMemberOf_uri_ms%3A"info:fedora/bc:barnard-literary-magazine"';
// See islandora_solr() in islandora_solr_search.module
$results = islandora_solr($query, NULL);
print t($results);
?>