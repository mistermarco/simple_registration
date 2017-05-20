<?php
/**
 * @file
 * Admin menus for the Simple Registration Module
 *
 */

function simple_registration_events_page() {

  // Show the current list of templates

  $header = array('Event', 'Operations');
  $rows = array();

  $result = db_query('SELECT eid, name FROM {simple_registration_events} ORDER BY name');

  foreach($result as $row) {
    $rows[] = array($row->name, l('edit', 'admin/config/simple_registration/events/edit/' . $row->eid));
  }

  $table = theme('table', array('header' => $header, 'rows' => $rows));

  $build = array();

  $build['template_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No events available. <a href="@link">Add event</a>.', array('@link' => url('admin/config/simple_registration/add'))),
  );

  return $build;
}


function simple_registration_events_add($form, &$form_state) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Event Name'),
    '#required' => TRUE,
    '#description' => t('Enter the name of the event.'),
  );

  $form['#submit'][] = 'simple_registration_events_add_submit';

  return system_settings_form($form);
}

function simple_registration_events_add_submit($form, &$form_state) {
  
  // Add template
  $id = db_insert('simple_registration_events')
    ->fields(array(
      'name' => $form_state['values']['name'],
    ))
  ->execute();

  $form_state['redirect'] =  "/admin/config/simple_registration/events";
}


function simple_registration_events_edit($form, &$form_state, $id) {

  $record = db_select('simple_registration_events', 'event')
    ->fields('event', array('eid', 'name'))
    ->condition('eid', $id, '=')
    ->execute()
    ->fetchAssoc();

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Event Name'),
    '#required' => TRUE,
    '#description' => t('Enter the name of the event.'),
    '#default_value' => $record['name'],
  );

  $form['eid'] = array(
    '#type' => 'hidden',
    '#value' => $record['eid'],
  );

  $form['#submit'][] = 'simple_registration_events_edit_submit';


  return system_settings_form($form);
}

function simple_registration_events_edit_submit($form, &$form_state) {
  
  // Add template
  $id = db_update('simple_registration_events')
    ->fields(array(
      'name' => $form_state['values']['name'],
    ))
    ->condition('eid', $form_state['values']['eid'], '=')
    ->execute();
}