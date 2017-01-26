<?php

/**
 * @file
 * Admin page callbacks for the content_theme module.
 */


/**
 * Menu callback; configures content wide themes.
 */
function content_theme_admin_content_wide($form, &$form_state) {

  $form['information'] = array(
    '#markup' => '<p>' . t('<em>Content wide themes</em> apply to all content and override the system default theme. But these settings can be overridden by content type and content node settings.') . '</p>',
  );

  $form['update'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update content wide themes'),
  );
  $form['update']['content_theme_edit'] = array(
    '#type' => 'select',
    '#title' => t('Creating & editing theme'),
    '#description' => t('Choose which theme the content creating and editing pages should display in. Default theme: %default_theme.', array('%default_theme' => t('System default theme'))),
    '#default_value' => config_get('content_theme.content_wide', 'content_theme_edit'),
    '#options' => content_theme_get_themes_options_content_wide(),
  );
  $form['update']['content_theme_view'] = array(
    '#type' => 'select',
    '#title' => t('Viewing theme'),
    '#description' => t('Choose which theme the content viewing pages should display in. Default theme: %default_theme.', array('%default_theme' => t('System default theme'))),
    '#default_value' => config_get('content_theme.content_wide', 'content_theme_view'),
    '#options' => content_theme_get_themes_options_content_wide(),
  );
  $form['update']['actions'] = array('#type' => 'actions');
  $form['update']['actions']['submit'] = array('#type' => 'submit', '#value' => t('Update themes'));

  return $form;
}

function content_theme_admin_content_wide_submit($form, &$form_state) {

  config('content_theme.content_wide')
    ->setData(array(
      'content_theme_edit' => $form_state['values']['content_theme_edit'],
      'content_theme_view' => $form_state['values']['content_theme_view'],
    ))
    ->save();

  backdrop_set_message(t('The themes have been updated.'));
}


/**
 * Menu callback; configures content type themes.
 */
function content_theme_admin_content_type($form, &$form_state) {

  $destination = backdrop_get_destination();
  $options = array();

  $header['type'] = t('Name');
  $header['theme_edit'] = t('Creating & editing theme');
  $header['theme_view'] = t('Viewing theme');
  $header['operations'] = t('Operations');

  foreach (node_type_get_names() as $name => $label) {

    $row['type'] = check_plain($label);
    $row['theme_edit'] = content_theme_get_theme_label_placeholder(config_get('node.type.' . $name, 'settings.content_theme_edit'));
    $row['theme_view'] = content_theme_get_theme_label_placeholder(config_get('node.type.' . $name, 'settings.content_theme_view'));
    $row['operations']['data'] = array(
      '#type' => 'operations',
      '#links' => array(
        'configure' => array(
          'title' => t('Configure'),
          'href' => 'admin/structure/types/manage/'. strtr($name, array('_' => '-')),
          'query' => $destination,
        ),
      ),
    );

    $options[$name] = $row;
  }


  $form['information'] = array(
    '#markup' => '<p>' . t('<em>Content type themes</em> apply to all content based on its content type and override content wide themes and the system default theme. But these settings can be overridden by content node settings.') . '</p>',
  );

  $form['update'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update content type themes'),
  );
  $form['update']['content_theme_edit'] = array(
    '#type' => 'select',
    '#title' => t('Creating & editing theme'),
    '#description' => t('Choose which theme the content creating and editing pages should display in. Default theme: %default_theme.', array('%default_theme' => t('Content wide theme'))),
    '#default_value' => '-no_update-',
    '#options' => array('-no_update-' => '- ' . t('No update') . ' -') + content_theme_get_themes_options_content_type(),
  );
  $form['update']['content_theme_view'] = array(
    '#type' => 'select',
    '#title' => t('Viewing theme'),
    '#description' => t('Choose which theme the content viewing pages should display in. Default theme: %default_theme.', array('%default_theme' => t('Content wide theme'))),
    '#default_value' => '-no_update-',
    '#options' => array('-no_update-' => '- ' . t('No update') . ' -') + content_theme_get_themes_options_content_type(),
  );
  $form['update']['actions'] = array('#type' => 'actions');
  $form['update']['actions']['submit'] = array('#type' => 'submit', '#value' => t('Update themes'));

  $form['types'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No content types available.'),
  );

  return $form;
}

function content_theme_admin_content_type_validate($form, &$form_state) {
  $values = $form_state['values'];

  if ($values['content_theme_edit'] == '-no_update-' && $values['content_theme_view'] == '-no_update-') {
    form_set_error('', t('No themes selected.'));
  }
  if (count(array_filter($values['types'])) == 0) {
    form_set_error('', t('Select one or more content type to perform the update on.'));
  }
}

function content_theme_admin_content_type_submit($form, &$form_state) {
  $values = $form_state['values'];

  foreach (array_filter($values['types']) as $name) {

    $config = config('node.type.' . $name);
    if ($values['content_theme_edit'] != '-no_update-') {
      $config->set('settings.content_theme_edit', $values['content_theme_edit']);
    }
    if ($values['content_theme_view'] != '-no_update-') {
      $config->set('settings.content_theme_view', $values['content_theme_view']);
    }
    $config->save();
  }

  node_type_cache_reset();

  backdrop_set_message(t('The themes have been updated.'));
}


/**
 * Menu callback; configures content node themes.
 */
function content_theme_admin_content_node($form, &$form_state) {

  $destination = backdrop_get_destination();
  $options = array();

  $header['title'] = array('data' => t('Title'), 'field' => 'n.title');
  $header['type'] = array('data' => t('Type'), 'field' => 'n.type');
  $header['theme_edit'] = t('Editing theme');
  $header['theme_view'] = t('Viewing theme');
  $header['changed'] = array('data' => t('Updated'), 'field' => 'n.changed', 'sort' => 'desc');
  $header['operations'] = t('Operations');

  $result = db_select('node', 'n')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('n', array('nid', 'title', 'type', 'changed'))
    ->limit(50)
    ->orderByHeader($header)
    ->execute();

  foreach ($result as $node) {

    $theme = db_query('SELECT theme FROM {content_theme_node} WHERE nid = :nid AND action = :action', array(':nid' => $node->nid, ':action' => 'edit'))->fetchField();
    $theme_edit = $theme !== FALSE ? $theme : '-content_type-';
    $theme = db_query('SELECT theme FROM {content_theme_node} WHERE nid = :nid AND action = :action', array(':nid' => $node->nid, ':action' => 'view'))->fetchField();
    $theme_view = $theme !== FALSE ? $theme : '-content_type-';

    $row['title'] = array(
      'data' => array(
        '#type' => 'link',
        '#title' => $node->title,
        '#href' => 'node/' . $node->nid,
      ),
    );
    $row['type'] = check_plain(node_type_get_name($node));
    $row['theme_edit'] = content_theme_get_theme_label_placeholder($theme_edit);
    $row['theme_view'] = content_theme_get_theme_label_placeholder($theme_view);
    $row['changed'] = format_date($node->changed, 'short');
    $row['operations']['data'] = array(
      '#type' => 'operations',
      '#links' => array(
        'configure' => array(
          'title' => t('Configure'),
          'href' => 'node/' . $node->nid . '/edit',
          'query' => $destination,
        ),
      ),
    );

    $options[$node->nid] = $row;
  }


  $form['information'] = array(
    '#markup' => '<p>' . t('<em>Content node themes</em> apply only to its own content and override the content type themes, content wide themes, and the system default theme.') . '</p>',
  );

  $form['update'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update content node themes'),
  );
  $form['update']['content_theme_edit'] = array(
    '#type' => 'select',
    '#title' => t('Editing theme'),
    '#description' => t('Choose which theme the content editing pages should display in. Default theme: %default_theme.', array('%default_theme' => t('Content type theme'))),
    '#default_value' => '-no_update-',
    '#options' => array('-no_update-' => '- ' . t('No update') . ' -') + content_theme_get_themes_options_content_node(),
  );
  $form['update']['content_theme_view'] = array(
    '#type' => 'select',
    '#title' => t('Viewing theme'),
    '#description' => t('Choose which theme the content viewing pages should display in. Default theme: %default_theme.', array('%default_theme' => t('Content type theme'))),
    '#default_value' => '-no_update-',
    '#options' => array('-no_update-' => '- ' . t('No update') . ' -') + content_theme_get_themes_options_content_node(),
  );
  $form['update']['actions'] = array('#type' => 'actions');
  $form['update']['actions']['submit'] = array('#type' => 'submit', '#value' => t('Update themes'));

  $form['nodes'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No content nodes available.'),
  );
  $form['pager'] = array(
    '#markup' => theme('pager'),
  );

  return $form;
}

function content_theme_admin_content_node_validate($form, &$form_state) {
  $values = $form_state['values'];

  if ($values['content_theme_edit'] == '-no_update-' && $values['content_theme_view'] == '-no_update-') {
    form_set_error('', t('Select one or both themes to perform the update on.'));
  }
  if (count(array_filter($values['nodes'])) == 0) {
    form_set_error('', t('Select one or more content node to perform the update on.'));
  }
}

function content_theme_admin_content_node_submit($form, &$form_state) {
  $values = $form_state['values'];

  foreach (array_filter($values['nodes']) as $nid) {

    if ($values['content_theme_edit'] != '-no_update-') {
      db_delete('content_theme_node')
        ->condition('nid', $nid)
        ->condition('action', 'edit')
        ->execute();
      if ($values['content_theme_edit'] != '-content_type-') {
        db_insert('content_theme_node')
          ->fields(array('nid' => $nid, 'action' => 'edit', 'theme' => $values['content_theme_edit']))
          ->execute();
      }
    }

    if ($values['content_theme_view'] != '-no_update-') {
      db_delete('content_theme_node')
        ->condition('nid', $nid)
        ->condition('action', 'view')
        ->execute();
      if ($values['content_theme_view'] != '-content_type-') {
        db_insert('content_theme_node')
          ->fields(array('nid' => $nid, 'action' => 'view', 'theme' => $values['content_theme_view']))
          ->execute();
      }
    }
  }

  cache_clear_all();

  backdrop_set_message(t('The themes have been updated.'));
}
