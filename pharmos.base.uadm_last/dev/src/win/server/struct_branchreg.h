/******************************************************************************/
/**                                                                           */
/**   Author : T.Hörath PRZ GmbH									          */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef ____STRUCT_BRANCHREG_H___
#define ____STRUCT_BRANCHREG_H___

/*----------------------------------------------------------------------------*/
struct T_ListCondppBranchReg
{
	short APPLICATIONID;
	bool  SELREG;
	bool  SELBRANCH;
	bool  SELCOR;
	long  USERID;
};

#define STRUCDESC_LISTCONDPPBRANCHREG	"sbbbl"

/*----------------------------------------------------------------------------*/
struct T_ListProjppBranchReg
{
    short   BRANCHID ;   
	short	REGIONID;
	char	BRANCHNAME	[L_PPBRANCH_NAME + 1];
	char	REGNAME		[L_PPREGION_NAME + 1];	
};

#define STRUCDESC_LISTPROJPPBRANCHREG	"ssc30c30"

/*----------------------------------------------------------------------------*/
struct T_ListProjppBranch
{
    short   BRANCHID ;   
	char	BRANCHNAME	[L_PPBRANCH_NAME + 1];
};

#define STRUCDESC_LISTPROJPPBRANCH	"sc30"

/*----------------------------------------------------------------------------*/
struct T_ListProjppRegion
{
   	short	REGIONID;
	char	REGNAME		[L_PPREGION_NAME + 1];	
};

#define STRUCDESC_LISTPROJPPREGION	"sc30"


#endif