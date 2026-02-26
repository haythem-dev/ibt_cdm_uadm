/******************************************************************************/
/**                                                                           */
/**   Author : T.Hörath PRZ GmbH									          */
/**   Date   : 15.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef ____STRUCT_APPRIGHT_H___
#define ____STRUCT_APPRIGHT_H___

/*----------------------------------------------------------------------------*/
struct T_ListCondppAppRight
{
	short APPLICATIONID;
	short MODULEID;
	short GROUPID;
	short USERID;
	short BRANCHREGID;
	bool  CORAPPRIGHT;
	bool  CORGROUPRIGHT;
	bool  MODRIGHTS;
	short CATEGORY;
};

#define STRUCDESC_LISTCONDPPAPPRIGHT	"sssssbbbs"

/*----------------------------------------------------------------------------*/
struct T_ListProjppAppRight
{
   	char    RIGHTNAME			[L_PPAPPRIGHT_RIGHTNAME + 1];
	char	RIGHTDESC			[L_PPAPPMODRIGHT_DESCRIPTION + 1];
	char	RIGHTDESC_EN		[L_PPAPPMODRIGHT_DESCRIPTION_EN + 1];
	char	RIGHTDESC_FR		[L_PPAPPMODRIGHT_DESCRIPTION_FR + 1];
	char	RIGHTDESC_HR		[L_PPAPPMODRIGHT_DESCRIPTION_HR + 1];
	short	CATEGORY;
};

#define STRUCDESC_LISTPROJPPAPPRIGHT	"c20c254c254c254c254s"


#endif