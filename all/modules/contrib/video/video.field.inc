<?php

/**
 * @file
 * Implement an video field, based on the file module's file field.
 */

/**
 * Implements hook_field_info().
 */
function video_field_info() {
  return array(
    'video' => array(
      'label' => t('Video'),
      'description' => t('This field stores the ID of an video file as an integer value.'),
      'settings' => array(
        'uri_scheme' => variable_get('file_default_scheme', 'public'),
        'autoconversion' => 0,
        'autothumbnail' => 'no',
        'default_video_thumbnail' => 0,
        'preview_video_thumb_style' => 'thumbnail',
      ),
      'instance_settings' => array(
        'file_extensions' => 'mp4 ogg avi mov wmv flv ogv webm',
        'file_directory' => 'videos/original',
        'max_filesize' => '',
        'default_dimensions' => '640x360',
      ),
      'default_widget' => 'video_upload',
      'default_formatter' => 'video',
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 */
function video_field_settings_form($field, $instance) {
  $defaults = field_info_field_settings($field['type']);
  $settings = array_merge($defaults, $field['settings']);
  $scheme_options = array();
  foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
    $scheme_options[$scheme] = $stream_wrapper['name'];
  }
  $form['uri_scheme'] = array(
    '#type' => 'radios',
    '#title' => t('Upload destination'),
    '#options' => $scheme_options,
    '#default_value' => $settings['uri_scheme'],
    '#description' => t('Select where the final files should be stored. Private file storage has significantly more overhead than public files, but allows restricted access to files within this field.'),
  );
  $form = $form + video_default_field_settings($settings);
  return $form;
}

/**
 * Element specific validation for video default value.
 *
 */
function video_field_default_thumbnail_validate($element, &$form_state) {
  $settings = $form_state['values']['field']['settings'];
  // Make the file permanent and store it in the form.
  if (!empty($settings['default_video_thumbnail']['fid'])) {
    $file = file_load($settings['default_video_thumbnail']['fid']);
    $file->status |= FILE_STATUS_PERMANENT;
    $file = file_save($file);
    $form_state['values']['field']['settings']['default_video_thumbnail'] = (array) $file;
  }
}

/**
 * Implements hook_field_instance_settings_form().
 */
function video_field_instance_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $instance_settings = $instance['settings'];
  $settings = $instance['settings'];
  // Use the file field instance settings form as a basis.
  $form = file_field_instance_settings_form($field, $instance);
  // Remove the description option.
  unset($form['description_field']);
  // add settings by widget type
  switch ($instance['widget']['type']) {
    case 'video_upload':
      break;
    case 'video_ftp':
      $form['ftp_path'] = array(
        '#type' => 'textfield',
        '#title' => t('FTP Filepath'),
        '#default_value' => !empty($widget['ftp_path']) ? $widget['ftp_path'] : 'ftpvideos',
        '#description' => t('The subdirectory within the "<em>files/</em>" directory where you have upload the videos for attachment.  Once the video is attached it will be moved from this directory to the main files directory.'),
        '#required' => TRUE,
        '#weight' => 3,
      );
      break;
  }
  //default settings
  $default = video_default_instance_settings($settings);
  $form = $default + $form;
  return $form;
}

/**
 * Implements hook_field_load().
 */
function video_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
  if ($field['settings']['autoconversion'] == 1) {
    //load the converted video
  }
  file_field_load($entity_type, $entities, $field, $instances, $langcode, $items, $age);
}

/**
 * Implements hook_field_prepare_view().
 */
function video_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
  // If there are no files specified at all, use the default.
  foreach ($entities as $id => $entity) {
    if (empty($items[$id]) && $field['settings']['default_video_thumbnail']['fid']) {
      if ($file = file_load($field['settings']['default_video_thumbnail']['fid'])) {
        $items[$id][0] = (array) $file + array(
          'is_default' => TRUE,
        );
      }
    }
  }
}

/**
 * Implements hook_field_presave().
 */
function video_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  // change the theumbnails if default is checked
  foreach ($items as $delta => $item) {
    if (!empty($field['settings']['default_video_thumbnail']['fid'])) {
      if ($item['use_default_video_thumb'])
        $items[$delta]['thumbnail'] = $field['settings']['default_video_thumbnail']['fid'];
    }
    // save thumbnail as permanent
    if (!empty($items[$delta]['thumbnail'])) {
      $file = file_load($items[$delta]['thumbnail']);
      $file->status |= FILE_STATUS_PERMANENT;
      $file = file_save($file);
    }
    // delete from cache if exists
    $cid = 'video:video:' . md5($item['fid']);
    $cache = cache_get($cid);
    if (!empty($cache->data)) {
      cache_clear_all($cid, 'cache');
    }
  }
  file_field_presave($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_insert().
 */
function video_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  file_field_insert($entity_type, $entity, $field, $instance, $langcode, $items);
  // calling function to handle conversion when auto conversion is enabled
  _video_field_file_autoconversion($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_update().
 */
function video_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  file_field_update($entity_type, $entity, $field, $instance, $langcode, $items);
  // calling function to handle conversion when auto conversion is enabled
  _video_field_file_autoconversion($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Video file save to the video_queue table for conversions
 */
function _video_field_file_autoconversion($entity_type, $entity, $field, $instance, $langcode, &$items) {
  // Create entry for video conversion if auto converison is enabled
  if ($field['settings']['autoconversion'] == 1) {
    $job = FALSE;
    switch ($entity_type) {
      case 'node':
        $entity_id = $entity->nid;
        break;
      case 'comment':
        $entity_id = $entity->cid;
        break;
    }
    // conversion class
    module_load_include('inc', 'video', 'includes/Conversion');
    $video_conversion = new Conversion();
    // transcoder factory
    $factory = new TranscoderAbstractionAbstractFactory();
    $transcoder = $factory->getProduct();
    foreach ($items as $delta => $item) {
      // set input file
      $transcoder->setInput($item);
      // skip adding entry if bypass conversion is checked
      if (isset($item['bypass_autoconversion']) && ($item['bypass_autoconversion'] == 1 || variable_get('video_bypass_conversion', FALSE))) {
        // delete the conversion job if any
        $transcoder->deleteJob($item);
        return;
      }
      // re queue for video conversion
      if (isset($item['re_convert_video']) && $item['re_convert_video'] == 1) {
        $video = $transcoder->loadJob();
        $video_conversion->changeStatus($video->vid, VIDEO_RENDERING_PENDING);
        $job = TRUE;
      }
      // Lets verify that we haven't added this video already.  Multiple validation fails will cause this to be ran more than once
      if (!$video = $transcoder->loadJob()) {
        if (!($transcoder->createJob($item, $entity_id, $entity_type)))
          drupal_set_message(t('Something went wrong with your video job creation.  Please check your recent log entries for further debugging.'), 'error');
        else
          $job = TRUE;
      }
      // if convert on save is checked
      if (isset($item['convert_video_on_save']) && $item['convert_video_on_save'] == 1) {
        // set to false when convert on save
        $job = FALSE;
        if ($video_conversion->process($item['fid'])) {
          drupal_set_message(t('Transcoding job submission was successful. Your video will convert and publish very soon.'));
        }
        else {
          $video = $transcoder->loadJob();
          $video_conversion->changeStatus($video->vid, VIDEO_RENDERING_FAILED);
          drupal_set_message(t('Something went wrong with your video transcoding.  Please check your recent log entries for further debugging.'), 'error');
        }
      }
    }
  }
}

/**
 * Implements hook_field_delete().
 */
function video_field_delete($entity_type, $entity, $field, $instance, $langcode, &$items) {
  file_field_delete($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_delete_revision().
 */
function video_field_delete_revision($entity_type, $entity, $field, $instance, $langcode, &$items) {
  file_field_delete_revision($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_is_empty().
 */
function video_field_is_empty($item, $field) {
  return file_field_is_empty($item, $field);
}

/**
 * Implements hook_field_widget_info().
 */
function video_field_widget_info() {
  return array(
    'video_upload' => array(
      'label' => t('Video Upload'),
      'field types' => array('video'),
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
//    'video_ftp' => array(
//      'label' => t('Video FTP'),
//      'field types' => array('video'),
//      'settings' => array(
//        'progress_indicator' => 'throbber',
//      ),
//      'behaviors' => array(
//        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
//        'default value' => FIELD_BEHAVIOR_NONE,
//      ),
//    ),
  );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function video_field_widget_settings_form($field, $instance) {
  // Use the file widget settings form.
  $form = file_field_widget_settings_form($field, $instance);

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function video_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  // Add display_field setting to field because file_field_widget_form() assumes it is set.
  $field['settings']['display_field'] = 0;

  $elements = file_field_widget_form($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);
  $settings = $instance['settings'];
  foreach (element_children($elements) as $delta) {

    // If not using custom extension validation, ensure this is an video.
    $supported_extensions = array_keys(video_video_extensions());
    $extensions = isset($elements[$delta]['#upload_validators']['file_validate_extensions'][0]) ? $elements[$delta]['#upload_validators']['file_validate_extensions'][0] : implode(' ', $supported_extensions);
    $extensions = array_intersect(explode(' ', $extensions), $supported_extensions);
    $elements[$delta]['#upload_validators']['file_validate_extensions'][0] = implode(' ', $extensions);

    // Add all extra functionality provided by the video widget.
    $elements[$delta]['#process'][] = 'video_field_widget_process';
  }

  if ($field['cardinality'] == 1) {
    // If there's only one field, return it as delta 0.
    if (empty($elements[0]['#default_value']['fid'])) {
      $elements[0]['#description'] = theme('file_upload_help', array('description' => $instance['description'], 'upload_validators' => $elements[0]['#upload_validators']));
    }
  }
  else {
    $elements['#file_upload_description'] = theme('file_upload_help', array('upload_validators' => $elements[0]['#upload_validators']));
  }
  return $elements;
}

/**
 * An element #process callback for the image_image field type.
 *
 * Expands the image_image type to include the alt and title fields.
 */
function video_field_widget_process($element, &$form_state, $form) {
  $item = $element['#value'];
  $item['fid'] = $element['fid']['#value'];
  $field = field_widget_field($element, $form_state);
  $instance = field_widget_instance($element, $form_state);
  $settings = $field['settings'];
  $element['#theme'] = 'video_widget';
  $element['preview'] = array();
  // Title is not necessary for each individual field.
  if ($field['cardinality'] != 1) {
    unset($element['#title']);
  }
  // add file elements by widget type
  switch ($instance['widget']['type']) {
    case 'video_upload':
      break;
    case 'video_ftp':
      break;
  }
  // Add our extra fields if in preview mode
  if (!empty($item['fid'])) {
    video_widget_element_settings($element, $form_state);
    // Create our thumbnails
    video_thumb_process($element, $form_state);
  }
  return $element;
}

/**
 * Formatters
 */

/**
 * Implementation of CCK's hook_field_formatter_info().
 */
function video_field_formatter_info() {
  $formatters = array(
    'video_formatter_player' => array(
      'label' => t('Video player'),
      'field types' => array('video'),
      'settings' => array(
        'widthxheight' => '640x360',
      ),
    ),
    'video_formatter_thumbnail' => array(
      'label' => t('Video thumbnail'),
      'field types' => array('video'),
      'settings' => array(
        'image_style' => '',
        'image_link' => ''
      ),
    ),
  );
  return $formatters;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function video_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  if ($display['type'] == 'video_formatter_thumbnail') {
    $image_styles = image_style_options(FALSE);
    $element['image_style'] = array(
      '#title' => t('Video thumbnail style'),
      '#type' => 'select',
      '#default_value' => $settings['image_style'],
      '#empty_option' => t('None (original video thumbnail)'),
      '#options' => $image_styles,
    );

    $link_types = array(
      'content' => t('Content'),
      'file' => t('File'),
    );
    $element['image_link'] = array(
      '#title' => t('Link video or video thumbanil to'),
      '#type' => 'select',
      '#default_value' => $settings['image_link'],
      '#empty_option' => t('Nothing'),
      '#options' => $link_types,
    );
  }
  // set the player wxh
  if ($display['type'] == 'video_formatter_player') {
    $element['widthxheight'] = array(
      '#title' => t('Dimensions'),
      '#type' => 'select',
      '#default_value' => $settings['widthxheight'],
      '#description' => t('Select the desired widthxheight of the video player. You can add your own dimention from !settings.', array('!settings' => l('video metadata settings', 'admin/config/media/video/metadata'))),
      '#options' => video_explode("\n", variable_get("video_metadata_dimensions", video_default_dimensions()))
    );
  }
  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function video_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $summary = array();
  $image_styles = image_style_options(FALSE);
  // Unset possible 'No defined styles' option.
  unset($image_styles['']);
  // Styles could be lost because of enabled/disabled modules that defines
  // their styles in code.
  if ($display['type'] == 'video_formatter_thumbnail') {
    if (isset($image_styles[$settings['image_style']])) {
      $summary[] = t('Video thumbnail style: @style', array('@style' => $image_styles[$settings['image_style']]));
    }
    else {
      $summary[] = t('Original video thumbnail');
    }

    $link_types = array(
      'content' => t('Linked to content'),
      'file' => t('Linked to video file'),
    );
    // Display this setting only if image is linked.
    if (isset($link_types[$settings['image_link']])) {
      $summary[] = $link_types[$settings['image_link']];
    }
  }
  if ($display['type'] == 'video_formatter_player') {
    $summary[] = t('WidthxHeight of the player: @widthxheight', array('@widthxheight' => $settings['widthxheight']));
  }

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function video_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $settings = $display['settings'];
  // set the display
  $theme = $display['type'];
  foreach ($items as $delta => $item) {
    if ($display['type'] == 'video_formatter_thumbnail') {
      // Check if the formatter involves a link.
      if ($settings['image_link'] == 'content') {
        $uri = entity_uri($entity_type, $entity);
      }
      elseif ($settings['image_link'] == 'file') {
        $link_file = TRUE;
      }
      if (isset($link_file)) {
        $uri = array(
          'path' => file_create_url($item['uri']),
          'options' => array(),
        );
      }
      $element[$delta] = array(
        '#theme' => $theme,
        '#item' => $item,
        '#image_style' => $settings['image_style'],
        '#path' => isset($uri) ? $uri : '',
        '#entity' => $entity,
        '#field' => $field,
        '#instance' => $instance
      );
    }
    if ($display['type'] == 'video_formatter_player') {
      $element[$delta] = array(
        '#theme' => $theme,
        '#item' => $item,
        '#entity' => $entity,
        '#field' => $field,
        '#instance' => $instance,
        '#player_dimensions' => $settings['widthxheight']
      );
    }
  }
  return $element;
}