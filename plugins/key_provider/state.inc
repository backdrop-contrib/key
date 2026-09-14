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
    'state_key' => '',
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
  $key_type = key_get_key_type_info($config['key_type']);

  $form['state_key'] = [
    '#type' => 'textfield',
    '#title' => t('State key'),
    '#description' => t('Name of the state variable.'),
    '#required' => TRUE,
    '#default_value' => $plugin_config['state_key'],
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
function key_provider_state_validate_configuration_form($form, &$form_state) {
  $state_variable = $form_state['values']['state_key'];
  $state_value = state_get($state_variable);
  if (!$state_value) {
    form_set_error('state_key', t('The state variable does not exist or it is empty.'));
    return;
  }
}

/**
 * Callback function to return a key from a file.
 */
function key_provider_state_get_key_value($config) {
  if (empty($config['key_provider_settings']['state_key'])) {
    return NULL;
  }
  $state_variable = $config['key_provider_settings']['state_key'];

  $key_value = state_get($state_variable);

  // Base64-decode if necessary.
  if (isset($config['key_provider_settings']['base64_encoded']) && $config['key_provider_settings']['base64_encoded']) {
    $key_value = base64_decode($key_value);
  }

  return $key_value;
}
