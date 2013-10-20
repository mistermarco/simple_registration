<?php

function simple_registration_drush_command() {
  $items = array();

  $items['simple-registration-register-user'] = array(
    'description' => 'Register a user',
	'arguments' => array(
	  'name' => 'The username of the user',
	),
    'options' => array(
      'notify' => array(
        'description'   => "Specify whether to notify the user",
        'example-value' => "--notify (notifies the user)",
      ),
    ),
	'aliases' => array('simreg'),
  );

  return $items;
}

function drush_simple_registration_register_user_validate($name = '') {
  $name = trim($name);

  // Can't register a user without a name
  if (empty($name)) {
    return drush_set_error(dt('Please specify a name.'));
  }

  // Check that there is a user with that name

  $user = user_load_by_name($name);

  if (!$user) {
    return drush_set_error(dt('User not found.'));
  } 

  $notify_user = drush_get_option('notify');

  if (!isset($notify_user)) { $notify_user = FALSE; }

  if ($notify_user === 'yes') {
    $notify_user = TRUE;
  }

  if ($notify_user !== TRUE) {
    $notify_user = FALSE;
  }

  $state = simple_registration_user_status($user->uid);
  if ($state == REGISTERED) {
    drush_log($name . ' is already registered.', 'ok');
	return FALSE;
  }
}

function drush_simple_registration_register_user($name = '') {
  $name = trim($name);

  $user = user_load_by_name($name);

  simple_registration_register_user($user->uid, FALSE, FALSE);
  drush_log($name . ' has been registered.', 'ok');
  watchdog('Simple Registration','Registerd user through drush: %user', array('%user' => $name));    
}

