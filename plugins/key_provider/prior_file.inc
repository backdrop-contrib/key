<?php

/**
 * @file
 * Plugin definition for the Prior File key provider.
 */

/**
 * The default plugin configuration.
 *
 * @return array
 *   The default plugin configuration.
 */
function key_provider_prior_file_default_configuration() {
  return array(
    'file_location' => '',
    'strip_line_breaks' => FALSE,
    'base64_encoded' => FALSE,
  );
}

/**
 * Build the plugin configuration form.
 *
 * @return array
 *   The plugin configuration form.
 */
function key_provider_prior_file_build_configuration_form($form, &$form_state) {
  $config = $form_state['storage']['key_config'];
  $plugin_config = $form_state['storage']['key_config']['key_provider_settings'] + key_provider_file_default_configuration();
  $key_type = key_get_key_type_info($config['key_type']);
  $is_new = empty($config['id']);

  $form['file_location'] = array(
    '#type' => 'textfield',
    '#title' => t('Pre-existing file location'),
    '#description' => t('The location of the text file. The recommended location is relative to the <a href="@filesystem_settings">Private file system path</a> (e.g., %str), if it has been set. Other options are to set an absolute path (e.g., %abs), or relative to the Backdrop directory (e.g., %rel). You will need to manually place this text file on your server with the key in it.',
      array(
        '@filesystem_settings' => url('admin/config/media/file-system'),
        '%abs' => '/etc/keys/foobar.key',
        '%rel' => '../keys/foobar.key',
        '%str' => 'private://keys/foobar.key',
      )
    ),
    '#default_value' => $plugin_config['file_location'],
    '#disabled' => !$is_new && $key_type['group'] == 'encryption',
  );

  $form['strip_line_breaks'] = array(
    '#type' => 'checkbox',
    '#title' => t('Strip trailing line breaks'),
    '#description' => t('Check this to remove any trailing line breaks from the file. Leave unchecked if there is a chance that a line break could be a valid character in the key.'),
    '#default_value' => $plugin_config['strip_line_breaks'],
  );

  // If this is for an encryption key.
  if ($key_type['group'] == 'encryption') {
    // Add an option to indicate that the value is Base64-encoded.
    $form['base64_encoded'] = array(
      '#type' => 'checkbox',
      '#title' => t('Base64-encoded'),
      '#description' => t('Check this if the key in the variable is Base64-encoded. <em>Note: Naturally Base64-encoded values, such as RSA keys, do not need to be marked as Base64-encoded unless they have been additionally encoded for another reason.</em>'),
      '#default_value' => $plugin_config['base64_encoded'],
    );
  }

  return $form;
}

/**
 * Validate callback for the configuration form.
 */
function key_provider_prior_file_validate_configuration_form($form, &$form_state) {
  $key_provider_settings = $form_state['values'];
  $filename = $key_provider_settings['file_location'];

  if (!$filename) {
    form_set_error('file_location', t('The file location is required.'));
    return;
  }

  // Does the file exist?
  if (!is_file($filename)) {
    form_set_error('file_location', t('There is no file at the specified location.'));
    return;
  }

  // Is the file readable?
  if ((!is_readable($filename))) {
    form_set_error('file_location', t('The file at the specified location is not readable.'));
    return;
  }
}

/**
 * Callback function to return a key from a file.
 */
function key_provider_prior_file_get_key_value($config) {
  if (empty($config['key_provider_settings']['file_location'])) {
    return NULL;
  }
  $filename = $config['key_provider_settings']['file_location'];

  // Make sure the file exists and is readable.
  if (!is_file($filename) || !is_readable($filename)) {
    return NULL;
  }

  $key_value = file_get_contents($filename);

  if (isset($config['key_provider_settings']['strip_line_breaks']) && $config['key_provider_settings']['strip_line_breaks']) {
    $key_value = rtrim($key_value, "\n\r");
  }

  // Base64-decode if necessary.
  if (isset($config['key_provider_settings']['base64_encoded']) && $config['key_provider_settings']['base64_encoded']) {
    $key_value = base64_decode($key_value);
  }

  return $key_value;
}

/**
 * Deletes the value of a key.
 *
 * @param array $config
 *   The key configuration whose value will be deleted.
 *
 * @return bool
 *   TRUE if successful, FALSE if unsuccessful.
 */
function key_provider_prior_file_delete_key_value($config) {
  // Do not delete pre-existing files.
  backdrop_set_message(t('The deleted key referenced a pre-existing key file. Pre-existing files are not deleted.'), 'warning');
  return TRUE;
}
