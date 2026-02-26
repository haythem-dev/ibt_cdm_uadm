#pragma once
#include <CConnect.h>
#ifndef __APIUSER_H____
#define __APIUSER_H____

struct ApiUser
{
	long		USERID;
	char		USERNAME[L_PPUSER_USERNAME + 1];
	char		SURNAME[L_PPUSER_SURNAME + 1];
	char		FORENAME[L_PPUSER_FORENAME + 1];
	char		PASSWORD[L_PPUSER_PASSWORD + 1];
	char		DESCRIPTION[L_PPUSER_DESCRIPTION + 1];
	char		MAILUSER[L_PPUSER_MAILUSER + 1];
	char		EMAILUSER[L_PPUSER_EMAILUSER + 1];
	short		CHANGEPW;
	short		ACTIV;
	long		LASTLOGIN;
	char		NTUSER[L_PPUSER_NTUSER + 1];
	long		LASTPWCHANGE;
	char		REFERENCE[L_PPUSER_REFERENCE + 1];
	short		EXTERNAL;
};

#endif // __APIUSER_H____