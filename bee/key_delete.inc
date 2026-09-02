<?php

use Drush\Log\LogLevel;

/**
 * Delete a key.
 *
 * @param array $arguments
 * @param array $options
 *
 * @return bool
 *   FALSE if not successful.
 */
function _bee_key_delete($arguments, $options) {
  // Look for a key with the specified ID. If one does not exist, set an
  // error and abort.
  $id = $arguments['id'] ?? NULL;
  $key = key_get_key($id);
  if (!$key) {
    bee_message(bt('Key !id does not exist.', array('!id' => $id)), 'error');
    return;
  }

  // Confirm that the key should be deleted.
  bee_instant_message(bt('Be extremely careful when deleting a key! It may result in losing access to a service or making encrypted data unreadable.'), 'warning');
  bee_instant_message(bt('The following key will be deleted: !id', array('!id' => $id)));
  if (!bee_confirm(bt('Do you really want to continue?'))) {
    return;
  }

  // Delete the key.
  key_delete_key($id, FALSE);

  // Try to load the key to confirm that it was deleted.
  $key_check = key_get_key($id);

  // If the key still exists, set an error and abort.
  if ($key_check) {
    bee_message(bt('Key !id was not deleted.', array('!id' => $id)), 'error');
    return;
  }

  bee_message(bt('Key !id was deleted successfully.', array('!id' => $id)), 'success');
}
