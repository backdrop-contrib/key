<?php

/**
 * @file
 * Key hooks implemented by the Key module.
 */

/**
 * Implements hook_key_type_info().
 */
function key_key_type_info() {
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

  $info['authentication'] = array(
    'label' => t('Authentication'),
    'description' => t('Use for a password or API key. (A generic key type that does not belong to any other defined key type.)'),
    'group' => 'authentication',
    'key value' => array(
      'plugin' => 'textarea_field',
    ),
  );

  return $info;
}

/**
 * Implements hook_key_input_info().
 */
function key_key_input_info() {
  $info['generate'] = array(
    'label' => t('Generate'),
    'description' => t('Generates a new key value.'),
    'default configuration' => 'key_input_generate_default_configuration',
    'build configuration form' => 'key_input_generate_build_configuration_form',
    'process submitted key value' => 'key_input_generate_process_submitted_key_value',
    'process existing key value' => '_key_default_process_existing_key_value',
    'file' => backdrop_get_path('module', 'key') . '/plugins/key_input/generate.inc',
  );

  $info['none'] = array(
    'label' => t('None'),
    'description' => t("A key input for providers that don't accept a value."),
    'build configuration form' => 'key_input_none_build_configuration_form',
    'file' => backdrop_get_path('module', 'key') . '/plugins/key_input/none.inc',
  );

  $info['text_field'] = array(
    'label' => t('Text field'),
    'description' => t('A simple text field.'),
    'default configuration' => 'key_input_text_field_default_configuration',
    'build configuration form' => 'key_input_text_field_build_configuration_form',
    'process submitted key value' => '_key_default_process_submitted_key_value',
    'process existing key value' => '_key_default_process_existing_key_value',
    'file' => backdrop_get_path('module', 'key') . '/plugins/key_input/text_field.inc',
  );

  $info['textarea_field'] = array(
    'label' => t('Textarea field'),
    'description' => t('A simple textarea field.'),
    'default configuration' => 'key_input_textarea_field_default_configuration',
    'build configuration form' => 'key_input_textarea_field_build_configuration_form',
    'process submitted key value' => '_key_default_process_submitted_key_value',
    'process existing key value' => '_key_default_process_existing_key_value',
    'file' => backdrop_get_path('module', 'key') . '/plugins/key_input/textarea_field.inc',
  );

  return $info;
}

/**
 * Implements hook_key_provider_info().
 */
function key_key_provider_info() {
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

  $info['env'] = array(
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
    'file' => backdrop_get_path('module', 'key') . '/plugins/key_provider/env.inc',
  );

  $info['state'] = array(
    'label' => t('State'),
    'description' => t('This key provider stores the key in the Backdrop state system. The state system stores values in the database, so keys stored this way should be considered to have the same security properties as database-stored configuration. This provider is suitable for development keys, API tokens with limited permissions, or other keys where database storage is acceptable.'),
    'storage method' => 'state',
    'key value' => array(
      'accepted' => TRUE,
      'required' => TRUE,
    ),
    'default configuration' => 'key_provider_state_default_configuration',
    'build configuration form' => 'key_provider_state_build_configuration_form',
    'validate configuration form' => 'key_provider_state_validate_configuration_form',
    'get key value' => 'key_provider_state_get_key_value',
    'set key value' => 'key_provider_state_set_key_value',
    'delete key value' => 'key_provider_state_delete_key_value',
    'file' => backdrop_get_path('module', 'key') . '/plugins/key_provider/state.inc',
  );

  $info['file'] = array(
    'label' => t('File'),
    'description' => t('The File key provider allows a key to be stored in a file, preferably outside of the web root.'),
    'storage method' => 'file',
    'key value' => array(
      'accepted' => FALSE,
      'required' => FALSE,
    ),
    'default configuration' => 'key_provider_file_default_configuration',
    'build configuration form' => 'key_provider_file_build_configuration_form',
    'validate configuration form' => 'key_provider_file_validate_configuration_form',
    'get key value' => 'key_provider_file_get_key_value',
    'file' => backdrop_get_path('module', 'key') . '/plugins/key_provider/file.inc',
  );

  return $info;
}
