<?php

/**
 * Get a list of available keys.
 *
 * @param array $arguments
 * @param array $options
 */
function _bee_key_list($arguments, $options) {
  $rows = array();

  $keys = key_get_keys();
  $key_types = key_get_plugins('key_type');
  $key_providers = key_get_plugins('key_provider');

  // Filter by key type, if specified.
  if (isset($options['key-type'])) {
    $key_type_filter = _convert_csv_to_array($options['key-type']);
    foreach ($keys as $id => $key) {
      if (!in_array($key['key_type'], $key_type_filter)) {
        unset($keys[$id]);
      }
    }
  }

  // Filter by key provider, if specified.
  if (isset($options['key-provider'])) {
    $key_provider_filter = _convert_csv_to_array($options['key-provider']);
    foreach ($keys as $id => $key) {
      if (!in_array($key['key_provider'], $key_provider_filter)) {
        unset($keys[$id]);
      }
    }
  }

  foreach ($keys as $id => $key) {
    $rows[] = array(
      array('value' => $id),
      array('value' => $key['label']),
      array('value' => $key_types[$key['key_type']]['label'] ?? ''),
      array('value' => $key_providers[$key['key_provider']]['label'] ?? ''),
    );
  }

  if (!empty($rows)) {
    return array(
      array(
        'type' => 'table',
        'variables' => array(
          'rows' => $rows,
          'header' => array(
            array('value' => bt('ID')),
            array('value' => bt('Label')),
            array('value' => bt('Key Type')),
            array('value' => bt('Key Provider')),
          ),
        ),
      ),
      array(
        'type' => 'text',
        'variables' => array(
          'value' => " Key List\n",
        ),
      ),
    );
  }
  else {
    bee_message(bt('No keys found.'), 'error');
    return;
  }
}

/**
 * Convert CSV to array
 *
 * @param string $args
 *
 * @return array
 */
function _convert_csv_to_array($args) {
  if ($args === NULL) {
    return array();
  }
  $args = is_array($args) ? implode(',', array_map('strval', $args)) : (string) $args;
  return array_map('trim', array_filter(explode(',', $args)));
}
