<?php

/**
 * @file
 * Plugin definition for the Private key provider.
 */

/**
 * The default plugin configuration.
 *
 * @return array
 *   The default plugin configuration.
 */
function key_provider_private_default_configuration() {
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
function key_provider_private_build_configuration_form($form, &$form_state) {
  $config = $form_state['storage']['key_config'];
  $plugin_config = $form_state['storage']['key_config']['key_provider_settings'] + key_provider_private_default_configuration();

  $form['file_location'] = array(
    '#type' => 'hidden',
    '#value' => $plugin_config['file_location'],
  );

  $form['strip_line_breaks'] = array(
    '#type' => 'checkbox',
    '#title' => t('Strip trailing line breaks'),
    '#description' => t('Check this to remove any trailing line breaks from the file. Leave unchecked if there is a chance that a line break could be a valid character in the key.'),
    '#default_value' => $plugin_config['strip_line_breaks'],
  );

  // If this is for an encryption key.
  $key_type = key_get_plugin('key_type', $config['key_type']);
  if ($key_type['group'] == 'encryption') {
    // Add an option to indicate that the value is Base64-encoded.
    $form['base64_encoded'] = array(
      '#type' => 'checkbox',
      '#title' => t('Base64-encoded'),
      '#description' => t('Checking this will store the key with Base64 encoding.'),
      '#default_value' => $plugin_config['base64_encoded'],
    );
  }

  return $form;
}

/**
 * Validate callback for the configuration form.
 */
function key_provider_private_validate_configuration_form($form, &$form_state) {
  $id = $form_state['storage']['key_config']['id'] ?? '';
  $config = config('key.key.' . $id);

  // Check that private file system is set.
  if (!config_get('system.core', 'file_private_path')) {
    form_set_error(NULL, t('The private file system needs to be set in order for the private key provider to save this key.'));
    return;
  }

  if ($config->isNew()) {
    $filename = _key_create_private_key_file();

    if (!$filename) {
      form_set_error(NULL, t('Unable to create the private key file.'));
      return;
    }

    $form_state['values']['file_location'] = $filename;
  }
  else {
    $filename = $form_state['values']['file_location'];

    // Make sure the file exists and is readable.
    if (!is_file($filename) || !is_readable($filename)) {
      form_set_error(NULL, t('Unable to read the private key file.'));
      return;
    }
  }
}

/**
 * Callback function to return a key from a file.
 */
function key_provider_private_get_key_value($config) {
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
 * Sets the value of a key.
 *
 * @param array $config
 *   The key configuration for which to set the value.
 * @param array $form_state
 *   The form state.
 * @param string $key_value
 *   The key value.
 *
 * @return bool
 *   TRUE if successful, FALSE if unsuccessful.
 */
function key_provider_private_set_key_value($config, &$form_state, $key_value) {
  if (isset($config['key_provider_settings']['base64_encoded']) && $config['key_provider_settings']['base64_encoded'] == TRUE) {
    $key_value = base64_encode($key_value);
  }

  // Ensure value isn't saved to the config.
  unset($form_state['values']['key_value']);

  $filename = '';
  if ($filename = $form_state['values']['file_location']) {
    $status = (bool) file_put_contents($filename, $key_value);
  }
  else {
    $status = FALSE;
  }

  return $status;
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
function key_provider_private_delete_key_value($config) {
  $filename = $config['key_provider_settings']['file_location'] ?? '';
  return $filename ? backdrop_unlink($filename) : FALSE;
}
