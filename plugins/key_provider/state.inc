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
  );
}

/**
 * Build the plugin configuration form.
 *
 * @return array
 *   The plugin configuration form.
 */
function key_provider_state_build_configuration_form($form, &$form_state) {
  $plugin_config = $form_state['storage']['key_config']['key_provider_settings'] + key_provider_state_default_configuration();

  $form['state_key'] = [
    '#type' => 'textfield',
    '#title' => t('State key'),
    '#description' => t('Name of the state variable.'),
    '#required' => TRUE,
    '#default_value' => $plugin_config['state_key'],
  ];

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
  return state_get($state_variable);
}
