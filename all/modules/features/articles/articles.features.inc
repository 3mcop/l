<?php
/**
 * @file
 * articles.features.inc
 */

/**
 * Implements hook_views_api().
 */
function articles_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_node_info().
 */
function articles_node_info() {
  $items = array(
    'lwts_article' => array(
      'name' => t('Lwts article'),
      'base' => 'node_content',
      'description' => t('The primary content type for Live With the Sea. May contain different kinds of media'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
