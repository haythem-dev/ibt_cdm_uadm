/******************************************************************************/
/**                                                                           */
/**   Author : T.Hörath PRZ GmbH									          */
/**   Date   : 03.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef ____STRUCT_PARA_H___
#define ____STRUCT_PARA_H___

/*----------------------------------------------------------------------------*/
struct T_ListCondppPara
{
	short APPLICATIONID;
};

#define STRUCDESC_LISTCONDPPPARA	"s"

/*----------------------------------------------------------------------------*/
struct T_ListProjppPara
{
   	char    PARA1          [L_PPLOGPARA_PARA1 + 1];
	char    PARA2          [L_PPLOGPARA_PARA2 + 1];
	char    PARABEZ        [L_PPLOGPARA_PARABEZ + 1];
	char    PARAVALUE      [L_PPLOGPARA_PARAVALUE + 1];
};

#define STRUCDESC_LISTPROJPPPARA	"c30c30c30c254"


#endif