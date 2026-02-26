/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef ____STRUCT_USERPARA_H___
#define ____STRUCT_USERPARA_H___

/*----------------------------------------------------------------------------*/
struct T_ListCondppUserPara
{
	short USERID;
	char  USERPARANAME[L_PPUSERPARA_NAME + 1];
	bool  ALLUSERPARAS;
	short SORTINDEX;
	bool  SORTORDER;
};

#define STRUCDESC_LISTCONDPPUSERPARA	"sc100bsb"

/*----------------------------------------------------------------------------*/
struct T_ListProjppUserPara
{
    short   USERPARAID;   
	char    NAME          [L_PPUSERPARA_NAME + 1];
	char    DESCRIPTION   [L_PPUSERPARA_DESC + 1];
	long    USERCOUNT;
};

#define STRUCDESC_LISTPROJPPUSERPARA	"sc100c254l"


#endif // ____STRUCT_USERPARA_H___
