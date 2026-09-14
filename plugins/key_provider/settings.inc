<?php

/**
 * @file
 * Plugin definition for the Settings key provider.
 */

/**
 * The default plugin configuration.
 *
 * @return array
 *   The default plugin configuration.
 */
function key_provider_settings_default_configuration() {
  return array(
    'settings_variable' => '',
    'base64_encoded' => FALSE,
  );
}

/**
 * Build the plugin configuration form.
 *
 * @return array
 *   The plugin configuration form.
 */
function key_provider_settings_build_configuration_form($form, &$form_state) {
  $config = $form_state['storage']['key_config'];
  $plugin_config = $form_state['storage']['key_config']['key_provider_settings'] + key_provider_settings_default_configuration();
  $key_type = key_get_key_type_info($config['key_type']);

  $form['settings_variable'] = [
    '#type' => 'textfield',
    '#title' => t('Settings variable'),
    '#description' => t('Name of the settings variable.'),
    '#required' => TRUE,
    '#default_value' => $plugin_config['settings_variable'],
  ];

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
function key_provider_settings_validate_configuration_form($form, &$form_state) {
  $settings_variable = $form_state['values']['settings_variable'];
  $settings_value = settings_get($settings_variable);
  if (!$settings_value) {
    form_set_error('settings_variable', t('The settings variable does not exist or it is empty. Make sure the variable is set in the <code>settings.php</code> file, in this format: <code>$settings[\'' . $settings_variable . '\'] = \'\';</code>.'));
    return;
  }
}

/**
 * Callback function to return a key from a file.
 */
function key_provider_settings_get_key_value($config) {
  if (empty($config['key_provider_settings']['settings_variable'])) {
    return NULL;
  }
  $settings_variable = $config['key_provider_settings']['settings_variable'];

  $key_value = settings_get($settings_variable);

  // Base64-decode if necessary.
  if (isset($config['key_provider_settings']['base64_encoded']) && $config['key_provider_settings']['base64_encoded']) {
    $key_value = base64_decode($key_value);
  }

  return $key_value;
}
