<?php

/**
 * Get a list of available key types.
 *
 * @param array $arguments
 * @param array $options
 */
function _bee_key_type_list($arguments, $options) {
  $rows = array();

  $group = $options['group'] ?? NULL;

  $plugins = key_get_plugins('key_type', TRUE);
  foreach ($plugins as $id => $plugin) {
    if (empty($group) || $plugin['group'] == $group) {
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
    bee_message(bt('No keys types found.'), 'error');
    return;
  }
}
