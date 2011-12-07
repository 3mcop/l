<?php
/**
 * @file
 * Wysiwyg module integration.
 */

/**
 * Implements hook_wysiwyg_include_directory().
 */
function wysiwyg_imagefield_wysiwyg_include_directory($type) {
  switch ($type) {
    case 'plugins':
      return $type;
  }
}
