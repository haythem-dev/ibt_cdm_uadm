/******************************************************************************/
/**                                                                           */
/**   Author : T.Hörath PRZ GmbH									          */
/**   Date   : 30.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef ____STRUCT_RIGHTANALYSIS_H___
#define ____STRUCT_RIGHTANALYSIS_H___

#include "tbl_ppbranch.h"
#include "tbl_ppapplication.h"
#include "tbl_ppuser.h"
#include "tbl_ppappmodule.h"

struct T_ListCondAnalysis
{
	short APPLICATIONID;
	short USERID;
	bool  USRONLY;
	bool  APPONLY;
	short BRANCHREGID;
};

#define STRUCDESC_LISTCONDANALYSIS	"ssbbs"

/*----------------------------------------------------------------------------*/
struct T_ListProjAnalysis
{
       char		APPNAME		[L_PPAPPLICATION_NAME + 1];
	   char		MODNAME		[L_PPAPPMODULE_NAME + 1];
	   char		USERNAME	[L_PPUSER_USERNAME + 1];
	   char		SURNAME		[L_PPUSER_SURNAME + 1];
	   char		FORENAME	[L_PPUSER_FORENAME + 1];
	   char		AREANAME	[L_PPBRANCH_NAME + 1];
	   char		RIGHTNAME	[L_PPAPPMODRIGHT_RIGHTNAME + 1];

		char	RIGHTDESC			[L_PPAPPMODRIGHT_DESCRIPTION + 1];
		char	RIGHTDESC_EN		[L_PPAPPMODRIGHT_DESCRIPTION_EN + 1];
		char	RIGHTDESC_FR		[L_PPAPPMODRIGHT_DESCRIPTION_FR + 1];
		char	RIGHTDESC_HR		[L_PPAPPMODRIGHT_DESCRIPTION_HR + 1];
};

#define STRUCDESC_LISTPROJANALYSIS	"c30c30c20c30c20c30c20c254c254c254c254"

#endif