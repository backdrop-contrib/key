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
  );
}

/**
 * Build the plugin configuration form.
 *
 * @return array
 *   The plugin configuration form.
 */
function key_provider_settings_build_configuration_form($form, &$form_state) {
  $plugin_config = $form_state['storage']['key_config']['key_provider_settings'] + key_provider_settings_default_configuration();

  $form['settings_variable'] = [
    '#type' => 'textfield',
    '#title' => t('Settings variable'),
    '#description' => t('Name of the settings variable.'),
    '#required' => TRUE,
    '#default_value' => $plugin_config['settings_variable'],
  ];

  return $form;
}

/**
 * Validate callback for the configuration form.
 */
function key_provider_settings_validate_configuration_form($form, &$form_state) {
  $settings_variable = $form_state['values']['settings_variable'];
  $settings_value = settings_get($settings_variable);
  if (!$settings_value) {
    form_set_error('settings_variable', t('The settings variable does not exist or it is empty.'));
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
  return settings_get($settings_variable);
}
