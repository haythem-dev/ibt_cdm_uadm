/******************************************************************************/
/**                                                                           */
/**   Author : T.Hörath PRZ GmbH									          */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef ____STRUCT_USERANALYSIS_H___
#define ____STRUCT_USERANALYSIS_H___

#include "tbl_ppbranch.h"
#include "tbl_ppapplication.h"
#include "tbl_ppuser.h"
#include "tbl_ppgroup.h"
#include "tbl_ppusertype.h"


struct T_ListCondUserGroupAnalysis
{
	short	APPLICATIONID;
    long	USERID;
};

#define STRUCDESC_LISTCONDUSERGROUPANALYSIS	"ss"

/*----------------------------------------------------------------------------*/
struct T_ListProjUserGroupAnalysis
{
    char		APPNAME     [L_PPAPPLICATION_NAME + 1];
    long        GROUPID;
    char		GROUPNAME   [L_PPGROUP_NAME + 1];
    char		USERNAME	[L_PPUSER_USERNAME + 1];
	char		SURNAME		[L_PPUSER_SURNAME + 1];
	char		FORENAME	[L_PPUSER_FORENAME + 1];
	char		USERCOUNTRY [L_PPUSER_COUNTRY + 1];
	char		USERTYPE    [L_PPUSERTYPE_NAME + 1];
};

#define STRUCDESC_LISTPROJUSERGROUPANALYSIS	"c30lc30c20c30c20c3c150"

#endif