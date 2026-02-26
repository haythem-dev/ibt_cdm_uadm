/******************************************************************************/
/**                                                                           */
/**   Author : T.Hörath PRZ GmbH									          */
/**   Date   : 24.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef ____STRUCT_USER_H___
#define ____STRUCT_USER_H___

#include "tbl_ppusertype.h"

struct T_ListCondppUser
{
	short APPLICATIONID;
	short GROUPID;
	bool  CORUSRGRP;
	short BRANCHREGID;
};

#define STRUCDESC_LISTCONDPPUSER	"ssbs"

/*----------------------------------------------------------------------------*/
struct T_ListProjppUser
{
       long		USERID;
	   char		USERNAME	[L_PPUSER_USERNAME + 1];
	   char		SURNAME		[L_PPUSER_SURNAME + 1];
	   char		FORENAME	[L_PPUSER_FORENAME + 1];
	   char		COUNTRY     [L_PPUSER_COUNTRY + 1];
	   char     USERTYPE    [L_PPUSERTYPE_NAME + 1];
};

#define STRUCDESC_LISTPROJPPUSER	"lc20c30c20c3c150"

/*----------------------------------------------------------------------------*/
struct T_SingleProjppUser
{
		char		PASSWORD	[L_PPUSER_PASSWORD + 1];
		char		DESCRIPTION	[L_PPUSER_DESCRIPTION + 1];
		char		MAILUSER	[L_PPUSER_MAILUSER + 1];
		char		EMAILUSER	[L_PPUSER_EMAILUSER + 1];
		short		CHANGEPW    ;
		short		ACTIV       ;
		long		LASTLOGIN   ;
		char		NTUSER		[L_PPUSER_NTUSER + 1];
		long		LASTPWCHANGE;
		char		REFERENCE	[L_PPUSER_REFERENCE + 1];
		short		EXTERNAL    ;
		char        PWD_ENC     [L_PPUSER_PWD_ENC + 1];
		char		COUNTRY     [L_PPUSER_COUNTRY + 1];
		short		USERTYPEID;
};

#define STRUCDESC_SINGLEPROJPPUSER	"c8c254c30c50sslc30lc40sc254c3s"


#endif