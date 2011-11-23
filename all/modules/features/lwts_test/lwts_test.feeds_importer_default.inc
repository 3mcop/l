<?php
/**
 * @file
 * lwts_test.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function lwts_test_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'csv_importer';
  $feeds_importer->config = array(
    'name' => 'CSV importer',
    'description' => 'import articles',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'lwts_article',
        'expire' => '-1',
        'author' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'body',
            'target' => 'body',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'Knowledge type',
            'target' => 'field_knowledge_type',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'Primary Topic',
            'target' => 'field_primary_topic',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'Origin',
            'target' => 'field_origin',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'plain_text',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['csv_importer'] = $feeds_importer;

  return $export;
}
