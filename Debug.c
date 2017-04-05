Debug()
{
	initTC();
//	creation(176);	//student creation
	login(3);		//login as an admin
	getdatabaseid();//getting id of user for creation. needs admin login first!
	creation2();	//creates a student (also with random country parameter)
//	tempshit();
	approval();		//approve student
	logout();
	initTC();
	login(2);		//login as a teacher
	enroll();		//enroll student at random course
	logout();
	initTC();
	login(1);		//login as a created student
	browse();		//student browse the course
	logout();
	
	
	return 0;
}
