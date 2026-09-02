<?php

/**
 * Get a list of available key providers.
 *
 * @param array $arguments
 * @param array $options
 */
function _bee_key_provider_list($arguments, $options) {
  $rows = array();

  $storage_method = $options['storage-method'] ?? NULL;

  $plugins = key_get_plugins('key_provider', TRUE);
  foreach ($plugins as $id => $plugin) {
    if (empty($storage_method) || $plugin['storage method'] == $storage_method) {
      $rows[] = array(
        array('value' => $id),
        array('value' => $plugin['description']),
      );
    }
  }

  if (!empty($rows)) {
    return array(
      array(
        'type' => 'table',
        'variables' => array(
          'rows' => $rows,
          'header' => array(
            array('value' => bt('ID')),
            array('value' => bt('Description')),
          ),
        ),
      ),
      array(
        'type' => 'text',
        'variables' => array(
          'value' => " Key Provider List\n",
        ),
      ),
    );
  } else {
    bee_message(bt('No keys providers found.'), 'error');
    return;
  }
}
