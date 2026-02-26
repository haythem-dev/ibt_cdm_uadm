/******************************************************************************/
/**                                                                           */
/**   Author : T.Hörath PRZ GmbH									          */
/**   Date   : 18.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef ____STRUCT_TEXT_H___
#define ____STRUCT_TEXT_H___


/*----------------------------------------------------------------------------*/
struct T_ListProjppText
{
    char    REFTEXT		  [L_PPTEXT_REFTEXT + 1];  
	char	TEXT		  [L_PPTEXT_TEXT + 1];
};

#define STRUCDESC_LISTPROJTEXT	"c254c240"

/*----------------------------------------------------------------------------*/
struct T_ListCondppText
{
	short APPLICATIONID;
	short MODULEID;
	char  LANGUAGEID [L_PPTEXT_LANGUAGEID + 1];
	bool  BCOR;
};

#define STRUCDESC_LISTCONDTEXT	"ssc2b"


#endif