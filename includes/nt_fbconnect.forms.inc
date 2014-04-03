<?php
/**
* Node, User form alter
*/


function nt_fbconnect_admin() {
  $form = array();
  $form['nt_fbconnect_appid'] = array(
    '#type' => 'textfield',
    '#title' => t('App ID'),
    '#default_value' => variable_get('nt_fbconnect_appid', ''),
    '#size' => 60,
    '#maxlength' => 100,    
    '#description' => t("Facebook App ID."),
    '#required' => TRUE,
    '#attributes' =>array('placeholder' => t('Enter you facebook app id'))
  );

  $form['nt_fbconnect_appsecret'] = array(
    '#type' => 'textfield',
    '#title' => t('App Secret'),
    '#default_value' => variable_get('nt_fbconnect_appsecret', ''),
    '#size' => 60,
    '#maxlength' => 100,
    '#description' => t("Facebook App Secret."),
    '#required' => TRUE,
    '#attributes' =>array('placeholder' => t('Enter you facebook app secret'))
  );
  return system_settings_form($form);
}