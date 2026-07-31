<?php

/**
 * @file
 * Hooks provided by the Key module
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Register a new key type plugin.
 *
 * @return array
 *   Array of plugins keyed by the plugin name, with the following keys:
 *   - label: The label of the plugin.
 *   - description: The description of the plugin.
 *   - group: The group for the key type.
 *   - key value: Contains information about the key value plugin used.
 *     - plugin: The key value plugin to use.
 *   - default configuration: The function to get the default plugin
 *     configuration.
 *   - build configuration form: The function to build the configuration form.
 *   - validate configuration form: The validation callback for the
 *     configuration form.
 *   - generate key value: The function to generate a key value.
 *   - validate key value: The function to validate a key value.
 *   - file: The file path where the plugin is defined.
 *
 * @see hook_key_type_info_alter()
 */
function hook_key_type_info() {
  $info['encryption'] = array(
    'label' => t('Encryption'),
    'description' => t('Can be used for encrypting and decrypting data. This key type has a field for selecting a key size, which is used to validate the size of the key value.'),
    'group' => 'encryption',
    'key value' => array(
      'plugin' => 'text_field',
    ),
    'default configuration' => 'key_type_encryption_default_configuration',
    'build configuration form' => 'key_type_encryption_build_configuration_form',
    'validate configuration form' => 'key_type_encryption_validate_configuration_form',
    'generate key value' => 'key_type_encryption_generate_key_value',
    'validate key value' => 'key_type_encryption_validate_key_value',
    'file' => backdrop_get_path('module', 'key') . '/plugins/key_type/encryption.inc',
  );

  return $info;
}

/**
 * Alter a key type plugin.
 *
 * @param array $key_types
 *   The associative array of key type plugins.
 *
 * @see hook_key_type_info()
 */
function hook_key_type_info_alter(&$key_types) {
  $key_types['encryption']['description'] = t('Updated description.');
}

/**
 * Register a new key input plugin.
 *
 * @return array
 *   Array of plugins keyed by the plugin name, with the following keys:
 *   - label: The label of the plugin.
 *   - description: The description of the plugin.
 *   - default configuration: The function to get the default plugin
 *     configuration.
 *   - build configuration form: The function to build the configuration form.
 *   - process submitted key value: The function to process a submitted key
 *     value.
 *   - process existing key value: The function to process an existing key
 *     value.
 *   - file: The file path where the plugin is defined.
 *
 * @see hook_key_input_info_alter()
 */
function hook_key_input_info() {
  $info['text_field'] = array(
    'label' => t('Text field'),
    'description' => t('A simple text field.'),
    'default configuration' => 'key_input_text_field_default_configuration',
    'build configuration form' => 'key_input_text_field_build_configuration_form',
    'process submitted key value' => '_key_default_process_submitted_key_value',
    'process existing key value' => '_key_default_process_existing_key_value',
    'file' => backdrop_get_path('module', 'key') . '/plugins/key_input/text_field.inc',
  );

  return $info;
}

/**
 * Alter a key input plugin.
 *
 * @param array $key_inputs
 *   The associative array of key input plugins.
 *
 * @see hook_key_input_info()
 */
function hook_key_input_info_alter(&$key_inputs) {
  $key_inputs['text_field']['description'] = t('Updated description.');
}

/**
 * Register a new key provider plugin.
 *
 * @return array
 *   Array of plugins keyed by the plugin name, with the following keys:
 *   - label: The label of the plugin.
 *   - description: The description of the plugin.
 *   - storage method: The storage method used by this provider.
 *   - key value: Contains information about key value handling.
 *     - accepted: Whether the provider accepts a key value.
 *     - required: Whether a key value is required.
 *   - default configuration: The function to get the default plugin
 *     configuration.
 *   - build configuration form: The function to build the configuration form.
 *   - validate configuration form: The function to validate the configuration
 *     form.
 *   - get key value: The function to retrieve a key value.
 *   - set key value: The function to set a key value.
 *   - delete key value: The function to delete a key value.
 *   - obscure key value: The function to obscure a key value.
 *   - file: The file path where the plugin is defined.
 *
 * @see hook_key_provider_info_alter()
 */
function hook_key_provider_info() {
  $info['config'] = array(
    'label' => t('Configuration'),
    'description' => t('The Configuration key provider stores the key in configuration. To make this relatively more secure set configuration directories to be outside the webroot (if using file storage of configuration, not database storage). And ensure any configuration backups and version control artifacts are protected. It is also possible to override the key value in <code>settings.php</code>. For example, <code>@format</code>. If using such an approach leave the key_value blank in the actual config record.', array('@format' => '$config[\'key.key.KEYNAME\'][\'key_provider_settings\'][\'key_value\'] = "THE OVERRIDDEN KEY VALUE";')),
    'storage method' => 'config',
    'key value' => array(
      'accepted' => TRUE,
      'required' => FALSE,
    ),
    'default configuration' => 'key_provider_config_default_configuration',
    'build configuration form' => 'key_provider_config_build_configuration_form',
    'get key value' => 'key_provider_config_get_key_value',
    'set key value' => 'key_provider_config_set_key_value',
    'delete key value' => 'key_provider_config_delete_key_value',
    'obscure key value' => 'key_provider_config_obscure_key_value',
    'file' => backdrop_get_path('module', 'key') . '/plugins/key_provider/config.inc',
  );

  return $info;
}

/**
 * Alter a key provider plugin.
 *
 * @param array $key_providers
 *   The associative array of key provider plugins.
 *
 * @see hook_key_provider_info()
 */
function hook_key_provider_info_alter(&$key_providers) {
  $key_providers['config']['description'] = t('Updated description.');
}

/**
 * @} End of "addtogroup hooks".
 */
