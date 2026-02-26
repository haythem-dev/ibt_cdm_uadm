/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef ____STRUCT_USERTYPE_H___
#define ____STRUCT_USERTYPE_H___

/*----------------------------------------------------------------------------*/
struct T_ListCondppUsertype
{
	short USERID;
	char  USERTYPENAME[L_PPUSERTYPE_NAME + 1];
	bool  ALLUSERTYPES;
	short SORTINDEX;
	bool  SORTORDER;
};

#define STRUCDESC_LISTCONDPPUSERTYPE	"sc150bsb"

/*----------------------------------------------------------------------------*/
struct T_ListProjppUsertype
{
    short   USERTYPEID;   
	char    NAME          [L_PPUSERTYPE_NAME + 1];
	char    DESCRIPTION   [L_PPUSERTYPE_DESC + 1];
	long    USERCOUNT;
};

#define STRUCDESC_LISTPROJPPUSERTYPE	"sc150c254l"


#endif