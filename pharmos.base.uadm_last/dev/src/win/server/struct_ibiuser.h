/******************************************************************************/
/**                                                                           */
/**   Author : T.Hörath PRZ GmbH									          */
/**   Date   : 06.06.2005                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef __STRUCT_IBIUSER_H__
#define __STRUCT_IBIUSER_H__

struct T_ListCondIbiUser
{
	char USERID		[L_IBI_USER_USERID + 1];
	char LASTNAME	[L_IBI_USER_LASTNAME + 1];
};

#define STRUCDESC_LISTCONDIBIUSER	"c50c30"

/*----------------------------------------------------------------------------*/
struct T_ListProjIbiUser
{
    short	BRANCHID;
	char	FIRSTNAME			[L_IBI_USER_FIRSTNAME + 1];
	char	REMARK				[L_IBI_USER_REMARK + 1];
	char	EMAIL				[L_IBI_USER_EMAIL + 1];
	char	DEFAULTCOUNTRYCODE	[L_IBI_USER_DEFAULTCOUNTRYCODE + 1];
	char	DEFAULTFORMAT		[L_IBI_USER_DEFAULTFORMAT + 1];
	char	DEFAULTSTYLE		[L_IBI_USER_DEFAULTSTYLE + 1];
};

#define STRUCDESC_LISTPROJIBIUSER	"sc20c50c50c3c50c50"

#define ATTRIBNO_LISTPROJIBIUSER	7

#endif