Debug()
{
	initTC();
	creation(173);	//student creation
	login(3);		//login as an admin
	//courseenrol();
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
