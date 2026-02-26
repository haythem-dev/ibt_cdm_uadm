/******************************************************************************/
/**                                                                           */
/**   Author : T.Hörath PRZ GmbH									          */
/**   Date   : 09.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef ____STRUCT_GROUP_H___
#define ____STRUCT_GROUP_H___

/*----------------------------------------------------------------------------*/
struct T_ListCondppGroup
{
	short APPLICATIONID;
	short USERID;
	bool  ALLGRP;
};

#define STRUCDESC_LISTCONDPPGROUP	"ssb"

/*----------------------------------------------------------------------------*/
struct T_ListProjppGroup
{
    long    GROUPID ;   
	char    NAME          [L_PPGROUP_NAME + 1];
};

#define STRUCDESC_LISTPROJPPGROUP	"lc30"


#endif