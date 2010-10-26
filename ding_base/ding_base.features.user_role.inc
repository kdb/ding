<?php

/**
 * Implementation of hook_user_default_roles().
 */
function ding_base_user_default_roles() {
  $roles = array();

  // Exported role: editor
  $roles['editor'] = array(
    'name' => 'editor',
  );

  return $roles;
}
