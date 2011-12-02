<?php
/**
 * @file
 * articles.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function articles_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

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
    'lwts_video' => array(
      'name' => t('Video'),
      'base' => 'node_content',
      'description' => t('An article containing a video. The body of the article may contain other HTML elements, e.g. images.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
