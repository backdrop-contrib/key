<?php

/**
 * @file
 * Plugin definition for the Private key provider.
 */

$plugin = array(
  'label' => t('Private file'),
  'description' => t(
    'This key provider stores the key in the <a href="@filesystem_settings">Private file system path</a> (e.g., %str).',
    array(
      '@filesystem_settings' => url('admin/config/media/file-system'),
      '%str' => 'private://keys/foobar.key',
    )
  ),
  'storage method' => 'file',
  'key value' => array(
    'accepted' => TRUE,
    'required' => FALSE,
  ),
  'default configuration' => 'key_provider_private_default_configuration',
  'build configuration form' => 'key_provider_private_build_configuration_form',
  'validate configuration form' => 'key_provider_private_validate_configuration_form',
  'get key value' => 'key_provider_private_get_key_value',
  'set key value' => 'key_provider_private_set_key_value',
  'delete key value' => 'key_provider_private_delete_key_value',
);

/**
 * The default plugin configuration.
 *
 * @return array
 *   The default plugin configuration.
 */
function key_provider_private_default_configuration() {
  return array(
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
  if ($id) {
    $file = "private://keys/{$id}.key";
  }

  // Check that private file system is set.
  if (!config_get('system.core', 'file_private_path')) {
    form_set_error(NULL, t('The private file system needs to be set in order for the private key provider to save this key.'));
    return;
  }

  // Only create if the file doesn't already exist.
  if (is_file($file)) {
    form_set_error(NULL, t('There is already a file at the specified location.'));
    return;
  }
}

/**
 * Callback function to return a key from a file.
 */
function key_provider_private_get_key_value($config) {
  $file = "private://keys/{$config['id']}.key";

  // Make sure the file exists and is readable.
  if (!is_file($file) || !is_readable($file)) {
    return NULL;
  }

  $key_value = file_get_contents($file);

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

  // Ensure it isn't saved to the config.
  unset($form_state['values']['key_value']);

  $file = "private://keys/{$config['id']}.key";

  return file_put_contents($file, $key_value);
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
  return backdrop_unlink("private://keys/{$config['id']}.key");
}
