/******************************************************************************/
/**                                                                           */
/**   Author : T.Hörath PRZ GmbH									          */
/**   Date   : 24.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef ____STRUCT_APPLICATION_H___
#define ____STRUCT_APPLICATION_H___


/*----------------------------------------------------------------------------*/
struct T_ListProjApp
{
    long    APPLICATIONID ;   
	char    NAME          [L_PPAPPLICATION_NAME + 1];
	char    DESCRIPTION   [L_PPAPPLICATION_DESCRIPTION + 1];
	char	NAME_EN		  [L_PPAPPLICATION_NAME_EN + 1];
	char	NAME_FR		  [L_PPAPPLICATION_NAME_FR + 1];
	char	NAME_HR		  [L_PPAPPLICATION_NAME_HR + 1];
};

#define STRUCDESC_LISTPROJAPP	"lc30c254c30c30c30"


#endif