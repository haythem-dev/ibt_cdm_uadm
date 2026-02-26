/******************************************************************************/
/**                                                                           */
/**   Author : T.Hörath PRZ GmbH									          */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef ____STRUCT_APPMODULE_H___
#define ____STRUCT_APPMODULE_H___

/*----------------------------------------------------------------------------*/
struct T_ListCondppAppMod
{
	short APPLICATIONID;
};

#define STRUCDESC_LISTCONDPPAPPMOD	"s"

/*----------------------------------------------------------------------------*/
struct T_ListProjppAppMod
{
    short   MODULEID ;   
	char    NAME          [L_PPAPPMODULE_NAME + 1];
	char	NAME_EN		  [L_PPAPPMODULE_NAME_EN + 1];
	char	NAME_FR		  [L_PPAPPMODULE_NAME_FR + 1];
	char	NAME_HR		  [L_PPAPPMODULE_NAME_HR + 1];
};

#define STRUCDESC_LISTPROJPPAPPMOD	"sc30c30c30"


#endif