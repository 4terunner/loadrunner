lib_Moodle()
{
	return (0);
}

void initTC()
{
	web_cleanup_cookies();
	web_cache_cleanup();
	web_set_sockets_option("SSL_VERSION", "2&3");
}	

void login(int userType)
{
char * userName; char * userPassword;
int i;
switch (userType)
{
	case 1:
		userName = lr_eval_string("{USER_NAME}");
		break;
	case 2:
		userName = "teacher1";
		break;
	case 3:
		userName = "admin";
		break;
}

	lr_save_string(userName, "USER_NAME");
	lr_save_string("Welcome1!", "USER_PASSWORD");
		
	/*Correlation comment - Do not change!  Original value='RRES6TigrZ' Name ='sesskey' Type ='ResponseBased'*/
	web_reg_save_param_regexp(
		"ParamName=sesskey",
		"RegExp=\\{\"wwwroot\":\"http:\\\\/\\\\/localhost\\\\/moodle\",\"sesskey\":\"(.*?)\",",
		SEARCH_FILTERS,
		"Scope=Body",
		"IgnoreRedirections=Yes",
		"RequestUrl=*/moodle/*",
		LAST);
	
	web_reg_find("Text=You are logged in as", "SaveCount=login_1", LAST);
	web_reg_find("Text=Logout", "SaveCount=login_2", LAST);
	web_reg_find("Text=Welcome to University of Greenland", "SaveCount=login_3", LAST);

	web_submit_data("index.php_2", 
		"Action=http://localhost/moodle/login/index.php", 
		"Method=POST", 
		"RecContentType=text/html", 
		"Referer=http://localhost/moodle/login/index.php", 
		"Snapshot=t21.inf", 
		"Mode=HTTP", 
		ITEMDATA, 
		"Name=username", "Value={USER_NAME}", ENDITEM, 
		"Name=password", "Value={USER_PASSWORD}", ENDITEM, 
		LAST);


	if ( atoi(lr_eval_string("{login_1}")) > 0 || atoi(lr_eval_string("{login_2}")) > 0 || atoi(lr_eval_string("{login_3}")) > 0)
	{
	lr_message("###############		Login is successful		###############");
	}
	else
	{
	lr_error_message("###############		Login is failed		###############");
	lr_exit(LR_EXIT_ITERATION_AND_CONTINUE, LR_FAIL);
	}
	
}

void creation(int id)
{
	char userName[100];
	char userEmail[100];
	//int id;
	lr_save_int(id + atoi(lr_eval_string("{ITER_NUMBER}")), "USER_ID");
	
	sprintf(userName, lr_eval_string("student{USER_ID}"));
	lr_save_string(userName, "USER_NAME");
	sprintf(userEmail, lr_eval_string("student{USER_ID}@mail.com"));
	lr_save_string(userEmail, "USER_EMAIL");
	
	lr_message("UserName:%s UserEmail:%s", userName, userEmail);
		
	/*Correlation comment - Do not change!  Original value='RRES6TigrZ' Name ='sesskey' Type ='ResponseBased'*/
	web_reg_save_param_regexp(
		"ParamName=sesskey",
		"RegExp=\\{\"wwwroot\":\"http:\\\\/\\\\/localhost\\\\/moodle\",\"sesskey\":\"(.*?)\",",
		SEARCH_FILTERS,
		"Scope=Body",
		"IgnoreRedirections=Yes",
		"RequestUrl=*/moodle/*",
		LAST);
	
	web_url("moodle", 
		"URL=http://localhost/moodle", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=", 
		"Snapshot=t123.inf", 
		"Mode=HTTP", 
		LAST);

	web_url("Login", 
		"URL=http://localhost/moodle/login/index.php", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://localhost/moodle/", 
		"Snapshot=t153.inf", 
		"Mode=HTTP", 
		LAST);
	
	web_reg_find("Text=An email should have been sent to your address at", "SaveCount=CreationSuccessful", LAST);
	
	web_submit_data("signup.php_2",
		"Action=http://localhost/moodle/login/signup.php",
		"Method=POST",
		"EncodeAtSign=YES",
		"RecContentType=text/html",
		"Referer=http://localhost/moodle/login/signup.php?",
		"Snapshot=t45.inf",
		"Mode=HTTP",
		ITEMDATA,
		"Name=sesskey", "Value={sesskey}", ENDITEM,
		"Name=_qf__login_signup_form", "Value=1", ENDITEM,
		"Name=username", "Value={USER_NAME}", ENDITEM,
		"Name=password", "Value=Welcome1!", ENDITEM,
		"Name=email", "Value={USER_EMAIL}", ENDITEM,
		"Name=email2", "Value={USER_EMAIL}", ENDITEM,
		"Name=firstname", "Value={USER_NAME}", ENDITEM,
		"Name=lastname", "Value={USER_NAME}", ENDITEM,
		"Name=city", "Value={USER_NAME}", ENDITEM,
		"Name=country", "Value=DZ", ENDITEM,
		"Name=submitbutton", "Value=Create my new account", ENDITEM,
		LAST);
	
	if ( atoi(lr_eval_string("{CreationSuccessful}")) > 0)
	{
	lr_message("###############		Student creation is successful		###############");
	}
	else
	{
	lr_error_message("###############		Student creation is failed		###############");
	//lr_exit(LR_EXIT_ITERATION_AND_CONTINUE, LR_FAIL);
	}
}

void enroll()
{
	/* on the course 101 page */
	
	char saveId[100];
	
	web_reg_save_param("ENROL_ID",
	"LB=enrolid\" value=\"", "RB=\" />", LAST);

	web_url("Enrolled users", 
		"URL=http://localhost/moodle/enrol/users.php?id={COURSE_ID}", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://localhost/moodle/course/view.php?id={COURSE_ID}", 
		"Snapshot=t598.inf", 
		"Mode=HTTP", 
		LAST);
	
	sprintf(saveId, lr_eval_string("{ENROL_ID}"));
	lr_save_string(saveId, "{ENROL_ID}");
	lr_message("###############		Enrol id: %s	###############", saveId);
	
	
	web_submit_data("ajax.php",
		"Action=http://localhost/moodle/enrol/ajax.php",
		"Method=POST",
		"RecContentType=application/json",
		"Referer=http://localhost/moodle/enrol/users.php?id={COURSE_ID}",
		"Snapshot=t613.inf",
		"Mode=HTTP",
		ITEMDATA,
		"Name=id", "Value={COURSE_ID}", ENDITEM,
		"Name=action", "Value=getassignable", ENDITEM,
		"Name=sesskey", "Value={sesskey}", ENDITEM,
		LAST);

	/* users page */

	lr_think_time(8);

	web_submit_data("ajax.php_3",
		"Action=http://localhost/moodle/enrol/manual/ajax.php",
		"Method=POST",
		"RecContentType=application/json",
		"Referer=http://localhost/moodle/enrol/users.php?id={COURSE_ID}",
		"Snapshot=t616.inf",
		"Mode=HTTP",
		ITEMDATA,
		"Name=id", "Value={COURSE_ID}", ENDITEM,
		"Name=sesskey", "Value={sesskey}", ENDITEM,
		"Name=action", "Value=searchusers", ENDITEM,
		"Name=search", "Value={USER_NAME}", ENDITEM,
		"Name=page", "Value=0", ENDITEM,
		"Name=enrolid", "Value={ENROL_ID}", ENDITEM,
		LAST);

	/* ^ found stud */

	web_submit_data("ajax.php_4",
		"Action=http://localhost/moodle/enrol/manual/ajax.php",
		"Method=POST",
		"RecContentType=application/json",
		"Referer=http://localhost/moodle/enrol/users.php?id={COURSE_ID}",
		"Snapshot=t620.inf",
		"Mode=HTTP",
		ITEMDATA,
		"Name=id", "Value={COURSE_ID}", ENDITEM,
		"Name=userid", "Value={USER_ID}", ENDITEM,
		"Name=enrolid", "Value={ENROL_ID}", ENDITEM,
		"Name=sesskey", "Value={sesskey}", ENDITEM,
		"Name=action", "Value=enrol", ENDITEM,
		"Name=role", "Value=5", ENDITEM,
		"Name=startdate", "Value=3", ENDITEM,
		"Name=duration", "Value=0", ENDITEM,
		"Name=recovergrades", "Value=0", ENDITEM,
		LAST);

	/* ^ hited enroll */

	lr_think_time(24);

	web_url("users.php", 
		"URL=http://localhost/moodle/enrol/users.php?id={COURSE_ID}&page=0&perpage=100&sort=lastname&dir=ASC", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://localhost/moodle/enrol/users.php?id={COURSE_ID}", 
		"Snapshot=t621.inf", 
		"Mode=HTTP", 
		LAST);

	web_submit_data("ajax.php_5",
		"Action=http://localhost/moodle/enrol/ajax.php",
		"Method=POST",
		"RecContentType=application/json",
		"Referer=http://localhost/moodle/enrol/users.php?id={COURSE_ID}&page=0&perpage=100&sort=lastname&dir=ASC",
		"Snapshot=t622.inf",
		"Mode=HTTP",
		ITEMDATA,
		"Name=id", "Value={COURSE_ID}", ENDITEM,
		"Name=action", "Value=getassignable", ENDITEM,
		"Name=sesskey", "Value={sesskey}", ENDITEM,
		LAST);

	/* ^ finished enrolling */
	
	
	web_reg_find("Text=Course profiles:","SaveCount=Enrolled", LAST);

	web_url("view.php", 
		"URL=http://localhost/moodle/user/view.php?id={USER_ID}&course={COURSE_ID}", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://localhost/moodle/enrol/users.php?id={COURSE_ID}&page=0&perpage=100&sort=lastname&dir=ASC", 
		"Snapshot=t623.inf", 
		"Mode=HTTP", 
		LAST);
	
	if (atoi(lr_eval_string("{Enrolled}")) > 0)
	{
	lr_message("###############		Student enrollment is successful	###############");
	}
	else
	{
	lr_error_message("###############		Student enrollment is failed	###############");
	//lr_exit(LR_EXIT_ITERATION_AND_CONTINUE, LR_FAIL);
	}
	/* ^ on user's page */

	
}

void logout()
{
	web_reg_find("Text=You are not logged in","SaveCount=LogoutCount", LAST);
	
		
	web_url("Logout",
		"URL=http://localhost/moodle/login/logout.php?sesskey={sesskey}",
		"Resource=0",
		"RecContentType=text/html",
		"Referer=http://localhost/moodle/",
		"Snapshot=t673.inf",
		"Mode=HTTP",
		LAST);
	
		if (atoi(lr_eval_string("{LogoutCount}")) > 0)
		{
		lr_message("###############		Logout is successful		###############");
		}
		else
		{
		lr_error_message("###############	Logout is failed		###############");
		}
}

void browse()
{
	web_reg_find("Text=http://localhost/moodle/course/view.php?id={COURSE_ID}", "SaveCount=Browsed", LAST);
	
	web_url("view.php", 
		"URL=http://localhost/moodle/course/view.php?id={COURSE_ID}", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://localhost/moodle/", 
		"Snapshot=t722.inf", 
		"Mode=HTTP", 
		LAST);
	
	if (atoi(lr_eval_string("{Browsed}")) > 0)
		{
		lr_message("###############		Course is browsed	###############");
		}
		else
		{
		lr_error_message("###############	Course browsing is failed	###############");
		}
}

void approval()
{

	web_reg_find("Text=http://localhost/moodle/admin/user.php?sort=name&dir=ASC&perpage=30&page=0&confirmuser={USER_ID}&sesskey={sesskey}", "SaveCount=UserConfirmed", LAST);
	
	web_url("Confirm",
		"URL=http://localhost/moodle/admin/user.php?sort=name&dir=ASC&perpage=30&page=0&confirmuser={USER_ID}&sesskey={sesskey}",
		"Resource=0",
		"RecContentType=text/html",
		"Referer=http://localhost/moodle/admin/user.php",
		"Snapshot=t919.inf",
		"Mode=HTTP",
		LAST);
		
		if (atoi(lr_eval_string("{UserConfirmed}")) == 0)
		{
		lr_message("###############		User is confirmed	###############");
		}
		else
		{
		lr_error_message("###############	User is not confirmed	###############");
		}

}

void courseenrol()
{
//	char somest [200];
//	int i;
//	
//	web_reg_save_param_ex("STUDENT_ID_SEARCH", "LB/ic=user/view.php?id=", "RB=&course=", LAST);
//	
//	web_url("view.php", 
//		"URL=http://localhost/moodle/admin/user.php?sort=firstname&dir=ASC&perpage=300&page=0", 
//		"Resource=0", 
//		"RecContentType=text/html", 
//		"Referer=http://localhost/moodle/", 
//		"Snapshot=t722.inf", 
//		"Mode=HTTP", 
//		LAST);
//	
//	for (i=0; i<sizeof(somest); i++)
//	{
//		sprintf(somest, lr_eval_string("{STUDENT_ID_SEARCH}"));
//		lr_save_string(somest, "{STUDENT_ID_SEARCH}");
//		lr_message("###############		Enrol id: %s	###############", somest);
//	}
	
	
//	char arrayParamName[300]; 
//	int elemCnt;
//	int x;
//	int y = 0;
//	char NIGGA [100];
//	//web_set_max_html_param_len("4000000");
//	web_reg_save_param_ex("ParamName=newParam", "LB/ic=href=\"../user/view.php?id=", "RB/ic=&amp;course=1\"", "Ordinal=all", LAST);
//
//
//	web_url("view.php", 
//		"URL=http://localhost/moodle/admin/user.php?sort=firstname&dir=ASC&perpage=300&page=0", 
//		"Resource=0", 
//		"RecContentType=text/html", 
//		"Referer=http://localhost/moodle/", 
//		"Snapshot=t722.inf", 
//		"Mode=HTTP", 
//		LAST);
//	
//	elemCnt = atoi(lr_eval_string("{newParam_count}"));
//	lr_save_string(atoi(lr_eval_string("{newParam_count}")+8), "NIGGA");//////////// TUT PIZDA
//	lr_output_message("Number of items found = %d",elemCnt);
//	
//	for (x=1;x<=elemCnt;x++) {
//		if (x > y) {
//			y = x;
//		}
//		else {
//			continue;
//		}
//	sprintf(arrayParamName, "{newParam_%d}", x);
//	lr_output_message ("%s: %s",arrayParamName,lr_eval_string(arrayParamName)); 
//	lr_output_message("%s", NIGGA);
//	}
//	
	

}