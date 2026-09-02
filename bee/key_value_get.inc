<?php

/**
 * Get a key value.
 *
 * TODO: Only return the value (without the index), so the label can be
 * translated, after figuring out why bt() returns an object, instead of
 * a string, for this command.
 *
 * @param array $arguments
 * @param array $options
 *   The ID of the key whose value should be retrieved.
 */
function _bee_key_value_get($arguments, $options) {
  // Look for a key with the specified ID. If one does not exist, set an
  // error and abort.
  $id = $arguments['id'] ?? NULL;
  $key = key_get_key($id);
  if (!$key) {
    bee_message(bt('Key !id does not exist.', array('!id' => $id)), 'error');
    return;
  }

  // Retrieve the key value.
  $key_value = key_get_key_value($id);

  // If the Base64 option was specified, encode the key value.
  $base64 = $options['base64'] ?? NULL;
  if ($base64) {
    $key_value = base64_encode($key_value);
  }

  bee_render_text(array('value' => $key_value));
}
