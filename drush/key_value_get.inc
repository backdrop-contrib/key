<?php

/**
 * Get a key value.
 *
 * TODO: Only return the value (without the index), so the label can be
 * translated, after figuring out why dt() returns an object, instead of
 * a string, for this command.
 *
 * @param string $id
 *   The ID of the key whose value should be retrieved.
 */
function _drush_key_value_get($id) {
  // Look for a key with the specified ID. If one does not exist, set an
  // error and abort.
  $key = key_get_key($id);
  if (!$key) {
    return drush_set_error('DRUSH_KEY_DOES_NOT_EXIST', dt('Key !id does not exist.', array('!id' => $id)));
  }

  // Retrieve the key value.
  $key_value = key_get_key_value($id);

  // If the Base64 option was specified, encode the key value.
  $base64 = drush_get_option('base64');
  if ($base64) {
    $key_value = base64_encode($key_value);
  }

  return array('Key value' => $key_value);
}
