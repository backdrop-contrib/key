<?php

/**
 * @file
 * Plugin definition for the State key provider.
 */

/**
 * The default plugin configuration.
 *
 * @return array
 *   The default plugin configuration.
 */
function key_provider_state_default_configuration() {
  return array(
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
function key_provider_state_build_configuration_form($form, &$form_state) {
  $config = $form_state['storage']['key_config'];
  $plugin_config = $form_state['storage']['key_config']['key_provider_settings'] + key_provider_state_default_configuration();

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
function key_provider_state_validate_configuration_form($form, &$form_state) {
  $is_new = empty($form_state['storage']['original_key']);
  $id = $form_state['storage']['key_config']['id'];
  if ($is_new) {
    $state_value = state_get('key_' . $id);
    if ($state_value) {
      form_set_error('state_key', t('The state key already exists. Use a different key name.'));
      return;
    }
    $form_state['values']['state_key'] = 'key_' . $id;
  }
}

/**
 * Callback function to return a key from a file.
 */
function key_provider_state_get_key_value($config) {
  $key_value = state_get('key_' . $config['id']);

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
function key_provider_state_set_key_value($config, &$form_state, $key_value) {
  if (isset($config['key_provider_settings']['base64_encoded']) && $config['key_provider_settings']['base64_encoded'] == TRUE) {
    $key_value = base64_encode($key_value);
  }

  // Ensure value isn't saved to the config.
  unset($form_state['values']['key_value']);

  state_set('key_' . $config['id'], $key_value);
  return TRUE;
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
function key_provider_state_delete_key_value($config) {
  state_del('key_' . $config['id']);
  return TRUE;
}
