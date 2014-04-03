<?php
/**
* helper function
*/


//require '/lib/facebook.php';
require_once("./".drupal_get_path('module','nt_fbconnect')."/lib/facebook.php");

/** Homepage Login Modal
* @retun form
* @author Nasir<nasir.mandal@sourcetop.com>
*/
function homepage_facebook_login() {
  	$loginForm = '';  

  	if(!variable_get('nt_fbconnect_appid')){
  		$loginForm .= '<p>You have to set your facebook api and secret.</p>';
  		$loginForm .= '<p>Go to <a href="admin/settings/fbconnect">setting page</a>';

  	} else {
	  // Create our Application instance (replace this with your appId and secret).
		$facebook = new Facebook(array(
		  'appId'  => variable_get('nt_fbconnect_appid'), //1390674701199313
		  'secret' => variable_get('nt_fbconnect_appsecret'), //92c98c6df51c81570d5604108432c5fd
		));

		// Get User ID
		$user = $facebook->getUser();

		if ($user) {
		  try {
		    // Proceed knowing you have a logged in user who's authenticated.
		    $user_profile = $facebook->api('/me');

		    $password = user_password(8);
		    $email = $user_profile['email'];

	    	$new_user = array(
			  'name' => $user_profile['name'],
			  'mail' => $user_profile['email'],
			  'pass' => $password,
			  'status' => 1,
			  'field_first_name' => array(LANGUAGE_NONE => array(array('value' => $user_profile['first_name']))),
			  'field_last_name' => array(LANGUAGE_NONE => array(array('value' => $user_profile['last_name']))),
			  'field_facebook_id' => array(LANGUAGE_NONE => array(array('value' => $user_profile['id']))),
			  'field_sex' => array(LANGUAGE_NONE => array(array('value' => $user_profile['gender']))),
			  'access' => REQUEST_TIME,
			  'roles' => array(
			      DRUPAL_AUTHENTICATED_RID => 'authenticated user'
			    )
			);

	    	
	    	if(user_load_by_mail($user_profile['email'])){
	    		$userLoad = user_load_by_mail($user_profile['email']);

	    		$edit['pass'] = $password;
				user_save($userLoad, $edit);

	    		nt_fbconnect_user_login_credentials($user_profile['name'], $password );
			    drupal_goto('user');
	    	}else{
	    		$account = user_save(NULL, $new_user);
	    		$account->password = $new_user['pass'];
	    		// Send the e-mail through the user module.
		  		//drupal_mail('user', 'register_no_approval_required', $email, NULL, array('account' => $account), variable_get('site_mail', 'noreply@example..com'));
		    	nt_fbconnect_user_login_credentials($user_profile['name'], $password);
		    	drupal_goto('user');
			}

		  } catch (FacebookApiException $e) {
		    error_log($e);
		    $user = null;
		  }
		}

		// Login or logout url will be needed depending on current user state.
		if ($user) {
		  $logoutUrl = $facebook->getLogoutUrl();
		} else {
		  $statusUrl = $facebook->getLoginStatusUrl();
		  $loginUrl = $facebook->getLoginUrl(array(
		                      'scope' => 'email'
		                      ));
		}

		if ($user){
	      $loginForm .= '<a href="'. $logoutUrl .'">Logout</a>';
	    } else{
	      $loginForm .= '<div>';
	      $loginForm .= '</div>';
	      $loginForm .= '<div>';
	        $loginForm .= '';
	        $loginForm .= '<a href="' . $loginUrl . '">Login with Facebook</a>';
	      $loginForm .= '</div>';
	    }

	    $loginForm .= '<hr/>';

	    if ($user){
	      $loginForm .= '<h3>You</h3>';
	      $loginForm .= '<img src="https://graph.facebook.com/' . $user . '/picture">';
	      
	    } else {
	      $loginForm .= '<strong><em>You are not Connected.</em></strong>';
	    }

		
	}

	return $loginForm;
}



