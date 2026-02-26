/******************************************************************************/
/**                                                                           */
/**   Author : T.Hörath PRZ GmbH									          */
/**   Date   : 18.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef ____STRUCT_LANGUAGE_H___
#define ____STRUCT_LANGUAGE_H___


/*----------------------------------------------------------------------------*/
struct T_ListProjLanguage
{
    char    LANGUAGEID	  [L_PPLOGINLANGUAGE_LANGUAGEID + 1];   
	char    NAME          [L_PPLOGINLANGUAGE_NAME + 1];
};

#define STRUCDESC_LISTPROJLANGUGAE	"c2c30"

/*----------------------------------------------------------------------------*/
struct T_ListCondLanguage
{
	short APPLICATIONID;
	bool  ALLLANG;
	bool  LANGLIST;
};

#define STRUCDESC_LISTCONDLANGUAGE	"sbb"


#endif