<?php
/**
 * @file
 * Define Wysiwyg ImageField Wysiwyg plugin.
 */

/**
 * Implements hook_INCLUDE_plugin().
 */
function wysiwyg_imagefield_wysiwyg_imagefield_plugin() {
  $plugins = array();

  // Define plugin if $_SESSION['wysiwyg_imagefield'] is set or page is Wysiwyg profile form.
  if (isset($_SESSION['wysiwyg_imagefield']) || strpos(request_uri(), 'admin/config/content/wysiwyg') >= 1) {
    $plugins['wysiwyg_imagefield'] = array(
      'title' => t('Wysiwyg ImageField'),
      'vendor url' => 'http://drupal.org/project/wysiwyg_imagefield',
      'icon file' => 'wysiwyg_imagefield.gif',
      'icon title' => t('Wysiwyg ImageField'),
      'settings' => array(),
    );

    unset($_SESSION['wysiwyg_imagefield']);
  }

  return $plugins;
}
