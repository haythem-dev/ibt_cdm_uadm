/******************************************************************************/
/**                                                                           */
/**   Author : T.Hörath PRZ GmbH									          */
/**   Date   : 18.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef ____STRUCT_PREVIOUS_H___
#define ____STRUCT_PREVIOUS_H___

#include "tbl_ppuser.h"
/*----------------------------------------------------------------------------*/
struct T_ListProjppPrev
{
    char    GLOBALUSER    [L_PPLOGINPREVIOUS_GLOBALUSER + 1];
	short	USERID;
	char	USERNAME	  [L_PPUSER_USERNAME + 1];
	char    LANGUAGEID	  [L_PPLOGINPREVIOUS_LANGUAGEID + 1];
	short	BRANCHREGIONID;
};

#define STRUCDESC_LISTPROJPREVIOUS	"c20sc20c2s"

/*----------------------------------------------------------------------------*/
struct T_ListCondppPrev
{
	short APPLICATIONID;
};

#define STRUCDESC_LISTCONDPREVIOUS	"s"


#endif