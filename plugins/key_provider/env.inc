<?php

/**
 * @file
 * Plugin definition for the Environment key provider.
 */

$plugin = array(
  'label' => t('Environment'),
  'description' => t('The Environment key provider allows a key to be stored in an environment variable.'),
  'storage method' => 'env',
  'key value' => array(
    'accepted' => FALSE,
    'required' => FALSE,
  ),
  'default configuration' => 'key_provider_env_default_configuration',
  'build configuration form' => 'key_provider_env_build_configuration_form',
  'validate configuration form' => 'key_provider_env_validate_configuration_form',
  'get key value' => 'key_provider_env_get_key_value',
);

/**
 * The default plugin configuration.
 *
 * @return array
 *   The default plugin configuration.
 */
function key_provider_env_default_configuration() {
  return array(
    'env_variable' => '',
    'base64_encoded' => FALSE,
    'strip_line_breaks' => TRUE,
  );
}

/**
 * Build the plugin configuration form.
 *
 * @return array
 *   The plugin configuration form.
 */
function key_provider_env_build_configuration_form($form, &$form_state) {
  $config = $form_state['storage']['key_config'];
  $plugin_config = $form_state['storage']['key_config']['key_provider_settings'] + key_provider_env_default_configuration();

  $form['env_variable'] = [
    '#type' => 'textfield',
    '#title' => t('Environment variable'),
    '#description' => t('Name of the environment variable.'),
    '#required' => TRUE,
    '#default_value' => $plugin_config['env_variable'],
  ];

  $form['strip_line_breaks'] = [
    '#type' => 'checkbox',
    '#title' => t('Strip trailing line breaks'),
    '#description' => t('Check this to remove any trailing line breaks from the variable. Leave unchecked if there is a chance that a line break could be a valid character in the key.'),
    '#default_value' => $plugin_config['strip_line_breaks'],
  ];

  // Add an option to indicate that the value is Base64-encoded.
  // This is always allowed since some hosted platforms use a web ui for
  // storing keys and do not allow multi-line values or special characters.
  $form['base64_encoded'] = array(
    '#type' => 'checkbox',
    '#title' => t('Base64-encoded'),
    '#description' => t('Check this if the key in the variable is Base64-encoded. <em>Note: Naturally Base64-encoded values, such as RSA keys, do not need to be marked as Base64-encoded unless they have been additionally encoded for another reason.</em>'),
    '#default_value' => $plugin_config['base64_encoded'],
  );

  return $form;
}

/**
 * Validate callback for the configuration form.
 */
function key_provider_env_validate_configuration_form($form, &$form_state) {
  $key_provider_settings = $form_state['values'];
  $env_variable = $key_provider_settings['env_variable'];
  $key_value = getenv($env_variable);

  // Does the env variable exist.
  if (!$key_value) {
    form_set_error('env_variable', t('The environment variable does not exist or it is empty.'));
    return;
  }
}

/**
 * Callback function to return a key from a file.
 */
function key_provider_env_get_key_value($config) {
  if (empty($config['key_provider_settings']['env_variable'])) {
    return NULL;
  }
  $env_variable = $config['key_provider_settings']['env_variable'];
  $key_value = getenv($env_variable);

  if (!$key_value) {
    return NULL;
  }

  if (isset($config['key_provider_settings']) && $config['key_provider_settings']['strip_line_breaks'] == TRUE) {
    $key_value = rtrim($key_value, "\n\r");
  }

  // Base64-decode if necessary.
  if (isset($config['key_provider_settings']['base64_encoded']) && $config['key_provider_settings']['base64_encoded']) {
    $key_value = base64_decode($key_value);
  }

  return $key_value;
}
