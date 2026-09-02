<?php

/**
 * Implements hook_bee_command().
 */
function key_bee_command() {
  $items['key-save'] = array(
    'description' => bt('Save a key.'),
    'callback' => 'key_save_bee_command',
    'group' => 'key',
    'examples' => array(
      'bee key-save secret_password \'pA$$w0rd\' --label="Secret password" --key-type=authentication --key-provider=config --key-input=text_field' => 'Define a key for a password to use for authentication using the Configuration key provider.',
      'bee key-save encryption_key --label="Encryption key" --key-type=encryption --key-type-settings=\'{"key_size":256}\' --key-provider=file --key-provider-settings=\'{"file_location":"private://keys/encryption.key", "base64_encoded":true}\' --key-input=none' => 'Define a key to use for encryption using the File key provider.',
    ),
    'arguments' => array(
      'id' => bt('The ID (machine name) of the key to save.'),
      'key_value' => bt('A key value to save. May or may not be allowed or required, depending on the key provider'),
    ),
    'required-arguments' => 1,
    'options' => key_bee_save_options(),
    'bootstrap' => BEE_BOOTSTRAP_FULL,
  );

  $items['key-delete'] = array(
    'description' => bt('Delete a key.'),
    'callback' => 'key_delete_bee_command',
    'group' => 'key',
    'arguments' => array(
      'id' => bt('The ID (machine name) of the key to delete.'),
    ),
    'required-arguments' => TRUE,
    'bootstrap' => BEE_BOOTSTRAP_FULL,
  );

  $items['key-list'] = array(
    'description' => bt('Display a list of available keys.'),
    'callback' => 'key_list_bee_command',
    'group' => 'key',
    'options' => array(
      'key-type' => array(
        'description' => bt('An optional, comma-delimited list of key types. To see a list of available key types, use `bee key-type-list`.'),
        'example-value' => 'authentication,encryption',
      ),
      'key-provider' => array(
        'description' => bt('An optional, comma-delimited list of key providers. To see a list of available key providers, use `bee key-provider-list`.'),
        'example-value' => 'config,file',
      ),
    ),
    'bootstrap' => BEE_BOOTSTRAP_FULL,
    'outputformat' => array(
      'default' => 'table',
      'pipe-format' => 'list',
      'field-labels' => array(
        'id' => 'ID',
        'label' => 'Label',
        'key_type' => 'Type',
        'key_provider' => 'Provider',
      ),
      'output-data-type' => 'format-table',
    ),
  );

  $items['key-type-list'] = array(
    'description' => bt('Display a list of available key types.'),
    'callback' => 'key_type_list_bee_command',
    'group' => 'key',
    'options' => array(
      'group' => array(
        'description' => bt('An optional key type group on which to filter.'),
        'example-value' => 'authentication,encryption',
      ),
    ),
    'bootstrap' => BEE_BOOTSTRAP_FULL,
    'outputformat' => array(
      'default' => 'table',
      'pipe-format' => 'list',
      'field-labels' => array(
        'id' => 'ID',
        'description' => 'Description',
      ),
      'output-data-type' => 'format-table',
    ),
  );

  $items['key-provider-list'] = array(
    'description' => bt('Display a list of available key providers.'),
    'callback' => 'key_provider_list_bee_command',
    'group' => 'key',
    'options' => array(
      'storage-method' => array(
        'description' => bt('An optional key provider storage method on which to filter.'),
        'example-value' => 'config,file',
      ),
    ),
    'bootstrap' => BEE_BOOTSTRAP_FULL,
    'outputformat' => array(
      'default' => 'table',
      'pipe-format' => 'list',
      'field-labels' => array(
        'id' => 'ID',
        'description' => 'Description',
      ),
      'output-data-type' => 'format-table',
    ),
  );

  $items['key-value-get'] = array(
    'description' => bt('Get a key value.'),
    'callback' => 'key_value_get_bee_command',
    'group' => 'key',
    'arguments' => array(
      'id' => bt('The ID (machine name) of the key whose value should be retrieved.'),
    ),
    'required-arguments' => TRUE,
    'options' => array(
      'base64' => bt('Base64-encode the key value. This is useful in the case of binary encryption keys that would otherwise not be displayed in a readable way.'),
      'pipe' => bt('Print just the key value, and nothing else.'),
    ),
    'aliases' => array('key-value'),
    'bootstrap' => BEE_BOOTSTRAP_FULL,
    'outputformat' => array(
      'default' => 'key-value',
      'pipe-format' => 'string',
      'label' => 'Key value',
      'output-data-type' => 'format-single',
    ),
  );

  return $items;
}

/**
 * Define the list of available bee options.
 *
 * @return array
 *   The list of bee options.
 */
function key_bee_save_options() {
  return array(
    'label' => array(
      'description' => bt('The human-readable label of the key.'),
    ),
    'description' => array(
      'description' => bt('A short description of the key.'),
    ),
    'key-type' => array(
      'description' => bt('The key type. To see a list of available key types, use `bee key-type-list`.'),
      'example-value' => 'authentication,encryption',
    ),
    'key-type-settings' => array(
      'description' => bt('Settings specific to the defined key type, in JSON format.'),
    ),
    'key-provider' => array(
      'description' => bt('The key provider. To see a list of available key providers, use `bee key-provider-list`.'),
      'example-value' => 'config,file',
    ),
    'key-provider-settings' => array(
      'description' => bt('Settings specific to the defined key provider, in JSON format.'),
    ),
    'key-input' => array(
      'description' => bt('The key input method.'),
      'example-value' => 'none,text_field',
    ),
    'key-input-settings' => array(
      'description' => bt('Settings specific to the defined key input, in JSON format.'),
    ),
    'overwrite' => 'Do not fail if the key already exists; overwrite it instead. Default is --no-overwrite.',
  );
}

/**
 * Command callback to save a key.
 */
function key_save_bee_command($arguments, $options) {
  require_once __DIR__ . '/key_save.inc';
  return _bee_key_save($arguments, $options);
}

/**
 * Command callback to delete a key.
 */
function key_delete_bee_command($arguments, $options) {
  require_once __DIR__ . '/key_delete.inc';
  return _bee_key_delete($arguments, $options);
}

/**
 * Command callback to get a list of available keys.
 */
function key_list_bee_command($arguments, $options) {
  require_once __DIR__ . '/key_list.inc';
  return _bee_key_list($arguments, $options);
}

/**
 * Command callback to get a list of available key types.
 */
function key_type_list_bee_command($arguments, $options) {
  require_once __DIR__ . '/key_type_list.inc';
  return _bee_key_type_list($arguments, $options);
}

/**
 * Command callback to get a list of available key providers.
 */
function key_provider_list_bee_command($arguments, $options) {
  require_once __DIR__ . '/key_provider_list.inc';
  return _bee_key_provider_list($arguments, $options);
}

/**
 * Command callback to get a key value.
 */
function key_value_get_bee_command($arguments, $options) {
  require_once __DIR__ . '/key_value_get.inc';
  return _bee_key_value_get($arguments, $options);
}
